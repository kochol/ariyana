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

		static void ReadFileJob(int _index, void* _userdata);

		core::Array<sx_job_t>	m_aJobs;
    };
} // namespace ari::io
