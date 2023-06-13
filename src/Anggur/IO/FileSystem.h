#pragma once

#include "Anggur/IO/Result.h"
#include <string>
#include <functional>

namespace Anggur::IO
{
	struct FileResult : Result
	{
		int descriptor;
	};

	struct FileStatisticResult : FileResult
	{
		size_t size;
	};

	struct FileContentResult : FileResult
	{
		size_t size;
		std::vector<unsigned char> buffer;
	};

	using FileCallback = std::function<void (const FileResult& result)>;

	using FileStatisticCallback = std::function<void (const FileStatisticResult& result)>;

	using FileContentCallback = std::function<void (const FileContentResult& result)>;

	void OpenFile(const std::string& path, int flags, int mode, const FileCallback& callback = nullptr);

	void CloseFile(int descriptor, const FileCallback& callback = nullptr);

	void GetFileStatistic(int descriptor, const FileStatisticCallback& callback = nullptr);

	/**
	 * The buffer will grow until hit EOF
	 */
	void ReadFile(int descriptor, size_t limit, size_t offset, const FileContentCallback& callback = nullptr);

	/**
	 * The buffer will stay the same as limit
	 */
	void ReadPartialFile(int descriptor, size_t limit, size_t offset, const FileContentCallback& callback = nullptr);

	/**
	 * The buffer will stay the same as limit only executed once
	 */
	void ReadPartialFileOnce(int descriptor, size_t limit, size_t offset, const FileContentCallback& callback = nullptr);
}
