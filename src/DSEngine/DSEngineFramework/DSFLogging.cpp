#define BOOST_LOG_DYN_LINK

#include <boost/log/sinks/debug_output_backend.hpp>
#include <boost/log/expressions/formatters/stream.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions/predicates/is_debugger_present.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/sinks/async_frontend.hpp>

// Related headers
#include <boost/log/sinks/unbounded_fifo_queue.hpp>
#include <boost/log/sinks/unbounded_ordering_queue.hpp>
#include <boost/log/sinks/bounded_fifo_queue.hpp>
#include <boost/log/sinks/bounded_ordering_queue.hpp>
#include <boost/log/sinks/drop_on_overflow.hpp>
#include <boost/log/sinks/block_on_overflow.hpp>

#include "DSFLogging.h"

namespace logging = boost::log;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;

typedef sinks::asynchronous_sink< sinks::text_ostream_backend > TextSink;
typedef sinks::asynchronous_sink< sinks::debug_output_backend > DebugSink;

boost::shared_ptr< TextSink > text_sink;
boost::shared_ptr< DebugSink > debug_sink;

void MessageFormatter(logging::record_view const& rec, logging::formatting_ostream& stream)
{
	std::stringstream ss0;
	std::string severity;
	ss0 << "<" << rec[logging::trivial::severity] << ">";
	ss0 >> severity;
	stream << std::setw(11) << std::right << severity;
	stream << std::setw(0) << std::left << "   ";

	std::stringstream ss1;
	std::string file;
	ss1 << "[" << logging::extract<std::string>("File", rec) << ':'
		<< logging::extract<int>("Line", rec) << ']';
	ss1 >> file;

	stream << std::setw(42) << std::left << file;

	stream << rec[expr::smessage];
}

void TextFormatter(logging::record_view const& rec, logging::formatting_ostream& stream)
{
	auto date_time_formatter = expr::stream << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f: ");
	date_time_formatter(rec, stream);
	MessageFormatter(rec, stream);
}

void DebugFormatter(logging::record_view const& rec, logging::formatting_ostream& stream)
{
	stream << "APPLICATION LOGGING: ";
	MessageFormatter(rec, stream);
	stream << std::endl;
}

void InitLogger()
{
	// Construct the sink
	text_sink = boost::make_shared< TextSink >();

	debug_sink = boost::make_shared< DebugSink >();

	logging::add_common_attributes();

	// Set formatter
	text_sink->set_formatter(&TextFormatter);

	debug_sink->set_formatter(&DebugFormatter);

	// Add a stream to write log to
	text_sink->locked_backend()->add_stream(
		boost::make_shared< std::ofstream >("engine.log"));

	// Set the special filter to the frontend
	// in order to skip the sink when no debugger is available
	debug_sink->set_filter(expr::is_debugger_present());

	// Register the sink in the logging core
	logging::core::get()->add_sink(text_sink);
	logging::core::get()->add_sink(debug_sink);
}

void StopLogger()
{
	boost::shared_ptr< logging::core > core = logging::core::get();

	// Remove the sink from the core, so that no records are passed to it
	core->remove_sink(text_sink);

	// Break the feeding loop
	text_sink->stop();

	// Flush all log records that may have left buffered
	text_sink->flush();

	text_sink.reset();


	core->remove_sink(debug_sink);

	debug_sink->stop();

	debug_sink->flush();

	debug_sink.reset();
}
