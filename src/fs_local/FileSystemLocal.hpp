#pragma once
#include "io/FileSystemBase.hpp"
#include "sx/jobs.h"

namespace ari::io
{
    class FileSystemLocal: public FileSystemBase
    {
    public:

		  FileSystemLocal();

	    void Update() override;

    private:

		static void ReadFileJob(int range_start, int range_end, int thread_index, void* _userdata);
		static bool ReadFileFromAssetsFolderAndroid(Request* request);

		core::Array<sx_job_t>	m_aJobs;
    };
} // namespace ari::io
