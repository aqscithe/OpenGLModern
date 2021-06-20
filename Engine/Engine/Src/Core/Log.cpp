#include <iostream>
#include <chrono>

#include <date/date.h>

#include <Core/Log.hpp>

using namespace Core::Debug;

bool Log::enabled = true;
std::ofstream Log::outMessage("Logs/message.log", std::ofstream::app);
std::streambuf* Log::coutbuf = std::cout.rdbuf(); //save old buf

std::ofstream Log::outError("Logs/error.log", std::ofstream::app);
std::streambuf* Log::cerrbuf = std::cerr.rdbuf();

void Log::redirectLogs(int type)
{
	switch (type)
	{
		case 0:
			std::cout.rdbuf(outMessage.rdbuf());
			break;
		case 1:
			std::cerr.rdbuf(outError.rdbuf());
			break;
		case 2:
			std::cout.rdbuf(outMessage.rdbuf());
			std::cerr.rdbuf(outError.rdbuf());
			break;
		default:
			std::string statement = "Invalid output stream code: " + std::to_string(type);
			Log::print("Invalid output stream code", static_cast<int>(LogType::ERROR));
			break;
	}
}

void Log::endRedirect(int type)
{
	switch (type)
	{
		case 0:
			std::cout.rdbuf(coutbuf);
			break;
		case 1:
			std::cout.rdbuf(cerrbuf);
			break;
		case 2:
			std::cout.rdbuf(outMessage.rdbuf());
			std::cerr.rdbuf(outError.rdbuf());
			break;
		default:
			std::string statement = "Invalid output stream code: " + std::to_string(type);
			Log::print("Invalid output stream code", static_cast<int>(LogType::ERROR));
			break;
	}
}

void    Log::configureLogFiles()
{
	redirectLogs(2);
	logHeader(static_cast<int>(LogFile::MESSAGE));
	logHeader(static_cast<int>(LogFile::ERROR));
	endRedirect(2);
}

void Log::logHeader(int logFile)
{
	logDivider(logFile);
	printTitle(logFile);
	printDateTime(logFile);
}

void Log::printTitle(int logFile)
{
	switch (logFile)
	{
		case static_cast<int>(LogFile::MESSAGE) :
			std::cout << "MESSAGE LOGS..." << std::endl;
			break;
		case static_cast<int>(LogFile::ERROR) :
			std::cerr << "ERROR LOGs..." << std::endl;
			break;
		default:
			print("Log::printTitle() - Not a valid log file code: " + std::to_string(logFile), static_cast<int>(LogType::ERROR));
			break;
	}
}

void Log::printDateTime(int logFile)
{
	auto tp = std::chrono::system_clock::now();
	auto dp = date::floor<date::days>(tp);
	auto ymd = date::year_month_day{ dp };
	auto time = date::make_time(std::chrono::duration_cast<std::chrono::milliseconds>(tp - dp));

	switch (logFile)
	{
		case static_cast<int>(LogFile::MESSAGE) :
			std::cout << ymd.year() << "-" << ymd.month() << "-" << ymd.day() << " " << time.hours().count()
				<< ":" << time.minutes().count() << ":" << time.seconds().count() << std::endl;
			std::cout << std::endl;
			break;
		case static_cast<int>(LogFile::ERROR) :
			std::cerr << ymd.year() << "-" << ymd.month() << "-" << ymd.day() << " " << time.hours().count()
				<< ":" << time.minutes().count() << ":" << time.seconds().count() << std::endl;
			std::cerr << std::endl;
			break;
		default:
			print("Log::printDateTime() - Not a valid log file code: " + std::to_string(logFile), static_cast<int>(LogType::ERROR));
			break;
	}
}

void Log::logDivider(int logFile)
{
	switch (logFile)
	{
		case static_cast<int>(LogFile::MESSAGE) :
			std::cout << "==========================================================================" << std::endl;
			std::cout << "==========================================================================" << std::endl;
			break;
		case static_cast<int>(LogFile::ERROR) :
			std::cerr << "==========================================================================" << std::endl;
			std::cerr << "==========================================================================" << std::endl;
			break;
		default:
			print("Log::logDivider() - Not a valid log file code: " + std::to_string(logFile), static_cast<int>(LogType::ERROR));
			break;
	}
	
}

void	Log::print(std::string statement, int logType)
{
	if (enabled)
	{
		switch (logType)
		{
			case static_cast<int>(LogType::INFO) :
				Log::redirectLogs(0);
				std::cout << "INFO LOG: " << statement << std::endl;
				Log::endRedirect(0);
				std::cout << "INFO LOG: " << statement << std::endl;
				break;
			case static_cast<int>(LogType::ERROR) :
				Log::redirectLogs(1);
				std::cerr << "ERROR LOG: " << statement << std::endl;
				Log::endRedirect(1);
				std::cerr << "ERROR LOG: " << statement << std::endl;
				exit(1);
				break;
			case static_cast<int>(LogType::WARNING) :
				Log::redirectLogs(1);
				std::cerr << "WARNING LOG: " << statement << std::endl;
				Log::endRedirect(1);
				std::cerr << "WARNING LOG: " << statement << std::endl;
				break;
			case static_cast<int>(LogType::FATAL) :
				Log::redirectLogs(1);
				std::cerr << "FATAL LOG: " << statement << std::endl;
				Log::endRedirect(1);
				std::cerr << "FATAL LOG: " << statement << std::endl;
				exit(1);
				break;
			case static_cast<int>(LogType::DEBUG) :
				Log::redirectLogs(0);
				std::cout << "DEBUG LOG: " << statement << std::endl;
				Log::endRedirect(0);
				std::cout << "DEBUG LOG: " << statement << std::endl;
				break;
			default:
				break;
		}
	}
	
}