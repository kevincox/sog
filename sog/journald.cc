#define SD_JOURNAL_SUPPRESS_LOCATION

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <fcntl.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <systemd/sd-journal.h>
// #include <unistd.h>

#include "journald.h"
#include "format.h"

namespace {

struct ToStringVisitor {
	std::string operator()(const long &s) const { return std::to_string(s); }
	std::string operator()(const double &s) const { return std::to_string(s); }
	std::string operator()(const std::string &s) const { return std::move(s); }
	std::string operator()(const std::experimental::string_view &s) const { return std::string(s); }
};

void write_u64le(std::ostream *out, uint64_t i) {
	for (size_t bit = 0; bit < 64; bit += 8)
		*out << (char)(i >> bit);
}

void write_record(
	std::ostream *out, std::experimental::string_view k, std::experimental::string_view v)
{
	*out << k << '\n';
	write_u64le(out, v.size());
	*out << v;
	*out << '\n';
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
	strcpy((char*)&addr.sun_path, "/run/systemd/journal/socket");
	auto r = connect(socket, (sockaddr*)&addr, sizeof(addr));
	if (r < 0) {
		perror("sog::JournaldSink connect(...)");
		abort();
	}
}

std::unique_ptr<sog::SinkData> sog::JournaldSink::prepare(const sog::Source *source) {
	return std::make_unique<Data>(source);
}

void sog::JournaldSink::log(sog::SinkData *sd, sog::Message msg) {
	Data *data = (Data*)sd;
	
	boost::iostreams::stream<boost::iostreams::file_descriptor> memfd(
		open("/dev/shm", O_CLOEXEC|O_TMPFILE|O_RDWR|O_EXCL), boost::iostreams::close_handle);
	
	write_record(&memfd, "MESSAGE", data->formatter.format(msg));
	write_record(&memfd, "SOG_TMPL", msg.source->msg_template);
	write_record(&memfd, "PRIORITY", std::to_string(msg.source->level));
	write_record(&memfd, "CODE_FILE", msg.source->file);
	write_record(&memfd, "CODE_FUNC", msg.source->function);
	write_record(&memfd, "CODE_LINE", std::to_string(msg.source->line));
	
	for (size_t i = 0; i < msg.source->value_count; ++i)
		write_record(
			&memfd,
			data->chunks[i],
			boost::apply_visitor(ToStringVisitor(), msg.values[i].data));
	
	msghdr msghdr{0};
	
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
	*((int *) CMSG_DATA(cmptr)) = memfd->handle();
	
	if (sendmsg(socket, &msghdr, MSG_NOSIGNAL) < 0)
		perror("sog::JournaldSink#log() sendmsg");
}
