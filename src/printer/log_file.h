#ifndef LOG_FILE_H
#define LOG_FILE_H

namespace ProjectName {

	class LogFile final
	{

	public:
		explicit LogFile() = default;
		LogFile(const LogFile&) = default;
		LogFile(LogFile&&) noexcept = default;
		virtual ~LogFile() = default;

		LogFile& operator=(const LogFile&) = default;
		LogFile& operator=(LogFile&&) noexcept = default;

		

	private:
	};

} // namespace ProjectName

#endif // LOG_FILE_H
