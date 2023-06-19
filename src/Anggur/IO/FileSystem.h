#pragma once

#include "Anggur/IO/Result.h"
#include <functional>
#include <string>

namespace Anggur {
	struct FileResult : Result {
		int descriptor;
	};

	struct FileStatisticResult : FileResult {
		size_t size;
	};

	struct FileContentResult : FileResult {
		size_t size;
		std::vector<unsigned char> buffer;
	};

	using FileCallback = std::function<void(const FileResult& result)>;

	using FileStatisticCallback = std::function<void(const FileStatisticResult& result)>;

	using FileContentCallback = std::function<void(const FileContentResult& result)>;

	void openFile(const std::string& path, int flags, int mode, const FileCallback& callback = nullptr);

	void closeFile(int descriptor, const FileCallback& callback = nullptr);

	void getFileStatistic(int descriptor, const FileStatisticCallback& callback = nullptr);

	/**
	 * The buffer will grow until hit EOF
	 */
	void readFile(int descriptor, size_t limit, size_t offset, const FileContentCallback& callback = nullptr);

	/**
	 * The buffer will stay the same as limit
	 */
	void readPartialFile(int descriptor, size_t limit, size_t offset, const FileContentCallback& callback = nullptr);

	/**
	 * The buffer will stay the same as limit only executed once
	 */
	void readPartialFileOnce(
		int descriptor, size_t limit, size_t offset, const FileContentCallback& callback = nullptr
	);

	void writeFile(
		int descriptor, size_t offset, std::vector<unsigned char> content, const FileCallback& callback = nullptr
	);
}
