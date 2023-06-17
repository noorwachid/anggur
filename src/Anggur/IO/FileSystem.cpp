#include "Anggur/IO/FileSystem.h"
#include "Anggur/IO/Internal.h"
#include <memory>
#include <iostream>

namespace Anggur
{
	void OpenFile(const std::string& path, int flags, int mode, const FileCallback& callback)
	{
		struct Data
		{
			uv_fs_t context;
			FileCallback callback;
		};

		auto data = new Data();
		data->context.data = data;
		data->callback = callback;

		uv_fs_open(uv_default_loop(), &data->context, path.c_str(), flags, mode, [](uv_fs_t* request)
		{
			auto data = static_cast<Data*>(request->data);

			if (data->callback)
			{
				FileResult result;
				result.state = request->result < 0 ? State::Error : State::Done;
				result.descriptor = request->result;
				data->callback(result);
			}

			uv_fs_req_cleanup(request);

			delete data;
		});
	}

	void CloseFile(int descriptor, const FileCallback& callback)
	{
		struct Data
		{
			uv_fs_t context;
			int descriptor;
			FileCallback callback;
		};

		auto data = new Data();
		data->context.data = data;
		data->descriptor = descriptor;
		data->callback = callback;

		uv_fs_close(uv_default_loop(), &data->context, descriptor, [](uv_fs_t* request)
		{
			auto data = static_cast<Data*>(request->data);

			if (data->callback)
			{
				FileResult result;
				result.state = request->result < 0 ? State::Error : State::Done;
				result.descriptor = data->descriptor;
				data->callback(result);
			}

			uv_fs_req_cleanup(request);

			delete data;
		});
	}

	void GetFileStatistic(int descriptor, const FileStatisticCallback& callback)
	{
		struct Data
		{
			uv_fs_t context;
			int descriptor;
			FileStatisticCallback callback;
		};

		auto data = new Data();
		data->context.data = data;
		data->descriptor = descriptor;
		data->callback = callback;

		uv_fs_fstat(uv_default_loop(), &data->context, descriptor, [](uv_fs_t* request)
		{
			auto data = static_cast<Data*>(request->data);

			uv_stat_t* stat = (uv_stat_t*) request->ptr;

			if (data->callback)
			{
				FileStatisticResult result;
				result.state = request->result < 0 ? State::Error : State::Done;
				result.descriptor = data->descriptor;
				result.size = stat->st_size;

				data->callback(result);
			}

			uv_fs_req_cleanup(request);

			delete data;
		});
	}

	namespace FileContentInternal
	{
		struct Data
		{
			uv_fs_t context;
			int descriptor;
			FileContentCallback callback;
			std::vector<unsigned char> buffer;
			std::vector<unsigned char> bufferComplete;
			uv_buf_t bufferContainer;
			size_t offset;

			Data(int descriptor, size_t limit)
			{
				this->descriptor = descriptor;
				buffer.resize(limit, 0);
				bufferContainer.base = reinterpret_cast<char*>(buffer.data());
				bufferContainer.len = buffer.size();
				offset = 0;
			}
		};

		void OnComplete(uv_fs_t* request)
		{
			auto data = static_cast<Data*>(request->data);

			if (request->result < 1)
			{
				if (data->callback)
				{
					FileContentResult result;
					result.state = request->result < 0 ? State::Error : State::Done;
					result.descriptor = data->descriptor;
					result.size = data->bufferComplete.size();
					result.buffer = data->bufferComplete;

					data->callback(result);
				}

				uv_fs_req_cleanup(request);

				delete data;
				
				return;
			}

			data->offset += request->result;

			if (data->callback)
			{
				FileContentResult result;
				result.state = State::InProgress;
				result.descriptor = data->descriptor;
				result.size = data->offset;
				
				data->bufferComplete.insert(data->bufferComplete.end(), data->buffer.begin(), data->buffer.begin() + request->result);

				data->callback(result);
			}

			uv_fs_req_cleanup(request);
			uv_fs_read(uv_default_loop(), request, data->descriptor, &data->bufferContainer, 1, data->offset, OnComplete);
		}

		void OnPartial(uv_fs_t* request)
		{
			auto data = static_cast<Data*>(request->data);

			if (request->result < 1)
			{
				if (data->callback)
				{
					FileContentResult result;
					result.state = request->result < 0 ? State::Error : State::Done;
					result.descriptor = data->descriptor;
					result.size = 0;

					data->callback(result);
				}

				uv_fs_req_cleanup(request);

				delete data;
				
				return;
			}

			if (data->callback)
			{
				FileContentResult result;
				result.state = State::InProgress;
				result.descriptor = data->descriptor;
				result.size = request->result;
				result.buffer = data->buffer;

				data->callback(result);
			}

			data->offset += request->result;

			uv_fs_req_cleanup(request);
			uv_fs_read(uv_default_loop(), request, data->descriptor, &data->bufferContainer, 1, data->offset, OnPartial);
		}

		void OnPartialOnce(uv_fs_t* request)
		{
			auto data = static_cast<Data*>(request->data);

			if (data->callback)
			{
				FileContentResult result;
				result.state = request->result < 0 ? State::Error : State::InProgress;
				result.descriptor = data->descriptor;
				result.size = request->result;
				result.buffer = data->buffer;

				data->callback(result);
			}

			uv_fs_req_cleanup(request);

			delete data;
		}
	}

	void ReadFile(int descriptor, size_t limit, size_t offset, const FileContentCallback& callback)
	{
		auto data = new FileContentInternal::Data(descriptor, limit);
		data->context.data = data;
		data->descriptor = descriptor;
		data->callback = callback;

		uv_fs_read(uv_default_loop(), &data->context, descriptor, &data->bufferContainer, 1, offset, FileContentInternal::OnComplete);
	}

	void ReadPartialFile(int descriptor, size_t limit, size_t offset, const FileContentCallback& callback)
	{
		auto data = new FileContentInternal::Data(descriptor, limit);
		data->context.data = data;
		data->descriptor = descriptor;
		data->callback = callback;

		uv_fs_read(uv_default_loop(), &data->context, descriptor, &data->bufferContainer, 1, offset, FileContentInternal::OnPartial);
	}

	void ReadPartialFileOnce(int descriptor, size_t limit, size_t offset, const FileContentCallback& callback)
	{
		auto data = new FileContentInternal::Data(descriptor, limit);
		data->context.data = data;
		data->descriptor = descriptor;
		data->callback = callback;

		uv_fs_read(uv_default_loop(), &data->context, descriptor, &data->bufferContainer, 1, offset, FileContentInternal::OnPartialOnce);
	}
}