#include <boost/algorithm/string/case_conv.hpp>
#include <boost/iostreams/stream.hpp>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "journald.h"

#include "format.h"

namespace {

struct ToStringVisitor {
	std::string operator()(const long &s) const { return std::to_string(s); }
	std::string operator()(const double &s) const { return std::to_string(s); }
	std::string operator()(const std::string &s) const { return std::move(s); }
	std::string operator()(const std::experimental::string_view &s) const { return std::string(s); }
};

void write_u64le(std::string *out, uint64_t i) {
	for (size_t bit = 0; bit < 64; bit += 8)
		*out += static_cast<char>(i >> bit);
}

void write_record(
	std::string *out, std::experimental::string_view k, std::experimental::string_view v)
{
	out->append(k.data(), k.size());
	*out += '\n';
	write_u64le(out, v.size());
	out->append(v.data(), v.size());
	*out += '\n';
}

std::vector<std::string> make_chunks(const sog::Source *source) {
	std::vector<std::string> r;
	r.reserve(source->value_count);
	
	for (size_t i = 0; i < source->value_count; ++i) {
		r.emplace_back(boost::algorithm::to_upper_copy(std::string(source->keys[i])));
	}
	
	return r;
}

struct Data: public sog::SinkData {
	sog::Formatter formatter;
	const std::vector<std::string> chunks;
	
	Data(const sog::Source *source):
		formatter{source},
		chunks{make_chunks(source)} {}
};
}

sog::JournaldSink::~JournaldSink() {
	if (socket < 0)
		return;
	
	auto r = close(socket);
	if (r < 0) {
		perror("~JournaldSink close(socket)");
		abort();
	}
}

sog::JournaldSink::JournaldSink() {
	socket = ::socket(AF_UNIX, SOCK_DGRAM|SOCK_CLOEXEC, 0);
	if (socket < 0) {
		perror("sog::JournaldSink socket(...)");
		abort();
	}
	
	sockaddr_un addr{AF_UNIX, "/run/systemd/journal/socket"};
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, "/run/systemd/journal/socket");
	auto r = connect(socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (r < 0) {
		perror("sog::JournaldSink connect(...)");
		abort();
	}
}

std::unique_ptr<sog::SinkData> sog::JournaldSink::prepare(const sog::Source *source) {
	return std::make_unique<Data>(source);
}

sog::Logged sog::JournaldSink::log(sog::SinkData *sd, sog::Message msg) {
	Data *data = static_cast<Data*>(sd);
	
	thread_local static std::string buf;
	buf.clear(); // Thread local to prevent allocations.
	
	write_record(&buf, "MESSAGE", data->formatter.format(msg));
	write_record(&buf, "SOG_TMPL", msg.source->msg_template);
	write_record(&buf, "PRIORITY", std::to_string(msg.source->level));
	write_record(&buf, "CODE_FILE", msg.source->file);
	write_record(&buf, "CODE_FUNC", msg.source->function);
	write_record(&buf, "CODE_LINE", std::to_string(msg.source->line));
	
	for (size_t i = 0; i < msg.source->value_count; ++i)
		write_record(
			&buf,
			data->chunks[i],
			boost::apply_visitor(ToStringVisitor(), msg.values[i].data));
	
	msghdr msghdr;
	memset(&msghdr, 0, sizeof(msghdr));
	
	iovec iov { const_cast<char*>(buf.data()), buf.size() };
	msghdr.msg_iov = &iov;
	msghdr.msg_iovlen = 1;
	
	auto r = sendmsg(socket, &msghdr, MSG_NOSIGNAL);
	if (r >- 0)
		return {};
	if (errno != EMSGSIZE && errno != ENOBUFS) {
		perror("sog::JournaldSink#log() sendmsg(data)");
		return {};
	}
	
	boost::iostreams::stream<boost::iostreams::file_descriptor> memfd(
		open("/dev/shm", O_CLOEXEC|O_TMPFILE|O_RDWR|O_EXCL, 0700),
		boost::iostreams::close_handle);
	memfd << buf;
	
	union {
		struct cmsghdr cm;
		char control[CMSG_SPACE(sizeof(int))];
	} control_un;
	msghdr.msg_control = control_un.control;
	msghdr.msg_controllen = sizeof(control_un.control);
	
	struct cmsghdr *cmptr;
	cmptr = CMSG_FIRSTHDR(&msghdr);
	cmptr->cmsg_len = CMSG_LEN(sizeof(int));
	cmptr->cmsg_level = SOL_SOCKET;
	cmptr->cmsg_type = SCM_RIGHTS;
	int fd = memfd->handle();
	memcpy(CMSG_DATA(cmptr), &fd, sizeof(fd));
	
	msghdr.msg_iovlen = 0;
	
	if (sendmsg(socket, &msghdr, MSG_NOSIGNAL) < 0)
		perror("sog::JournaldSink#log() sendmsg(fd)");
	
	return {};
}
