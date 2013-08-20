#pragma once

class CMainFrame;

extern CMainFrame* main_frame;

class debug_kernel
{
public:
	debug_kernel(void);
	~debug_kernel(void);

public:
	enum debug_status
	{
		STOP,
		RUN,
		BREAK
	};

	struct  breakpoint
	{
		DWORD address;	//�ϵ��ַ
		byte org_data;	//�ϵ㴦��ԭʼ����
		bool valid;	//�ϵ�ʵ�����Ƿ�������
		bool user_enable;	//�û��Ƿ������˸öϵ�

		int hits;	// ���д���
		bool is_once; // �Ƿ���һ���Ե�
	};

	// ����һ��exe����ʼ����
	bool load_exe(std::string& exe_path, std::string& command_str,std::string& current_path);
	// ���ӵ�ָ���Ľ��̲���ʼ����
	bool attach_process(DWORD pid);

	// ��������
	bool step_in();
	// ��������
	bool step_over();

	// �������ԣ����б����Խ���
	bool continue_debug(DWORD continue_status)
	{
		breakpoint* bp = find_breakpoint_by_address(context_.Eip);
		if (bp)
		{
			invalid_breakpoint(bp);
		}


		return SetEvent(continue_event_);
	}

	bool add_breakpoint(DWORD address,bool is_once = false);


	bool read_memory(DWORD address,void* buffer,size_t size,SIZE_T* num_read = NULL );
	bool write_memory(DWORD address,void* data,SIZE_T size, SIZE_T* num_written = NULL);
	bool modify_memory_prop(DWORD address,SIZE_T size,DWORD new_protect,DWORD* old_protect = NULL);
	bool virtual_query_ex(DWORD address,MEMORY_BASIC_INFORMATION& info);
	breakpoint* find_breakpoint_by_address(DWORD address);
	// �û����öϵ�
	bool enable_breakpoint(breakpoint* bp);
	bool enable_breakpoint(DWORD address);
	// �û����öϵ�
	bool disable_breakpoint(breakpoint* bp);
	bool disable_breakpoint(DWORD address);
	// ʹ�ϵ���Ч
	bool valid_breakpoint(breakpoint* bp);
	bool valid_breakpoint(DWORD address);
	// ʹ�ϵ���Ч��Ϊ�˽��е����Ȳ���ʱ���ܶϵ��Ӱ�죬�������ǽ��öϵ㣩
	bool invalid_breakpoint(breakpoint* bp);
	bool invalid_breakpoint(DWORD address);
	bool delete_breakpoint(DWORD address);
	void update_breakpoint_starus()
	{
		for (int i=0;i<bp_vec_.size();++i)
		{
			breakpoint& bp = bp_vec_[i];
			if (bp.user_enable == true && bp.valid == false)
			{
				valid_breakpoint(&bp);
			}
			else if(bp.user_enable == true && bp.valid == true)
			{
				invalid_breakpoint(&bp);
			}
		}
	}


private:
	// ���һ��WaitForDebugEvent��ȡ���ĵ����¼�
	DEBUG_EVENT debug_event_;
	// ���һ�ε����¼�ʱ���߳�context
	CONTEXT context_;
	// �쳣�Ƿ��Ѿ�����
	DWORD	continue_status_;
	// �����������õ��¼�
	HANDLE continue_event_;
	//HANDLE process_handle_;
	bool debugee_exit_;
	//DWORD process_id_;
	debug_status debug_status_;

	std::vector<breakpoint> bp_vec_;

	// �����Խ��̵Ľ��̾��
	HANDLE handle_;
	// �����Խ��̵Ľ���ID
	DWORD pid_;


// 	std::string exe_path_;
// 	std::string command_str_;
// 	std::string current_path_;

private:
	void debug_thread_proc();
	bool on_create_process_event(const CREATE_PROCESS_DEBUG_INFO& create_process_info);
	bool on_exit_process_event(const EXIT_PROCESS_DEBUG_INFO& exit_process);
	bool on_create_thread_event(const CREATE_THREAD_DEBUG_INFO& create_thread);
	bool on_exit_thread_event(const EXIT_THREAD_DEBUG_INFO& exit_thread);
	bool on_load_dll_event(const LOAD_DLL_DEBUG_INFO& load_dll);
	bool on_unload_dll_event(const UNLOAD_DLL_DEBUG_INFO& unload_dll);
	bool on_output_debug_string_event(const OUTPUT_DEBUG_STRING_INFO& debug_string);
	bool on_rip_event(const RIP_INFO& rip_info);
	bool on_exception_event(const EXCEPTION_DEBUG_INFO& debug_exception);


////////////////////////////////////////////////////////////////////////////////////////////////
	//memory map���
public:
	struct section_info_t
	{
		char	name[IMAGE_SIZEOF_SHORT_NAME+1];
		byte*	start_addr;
		DWORD	size;
	};

	struct module_info_t 
	{
		BYTE  * module_base_addr;        // Base address of module in th32ProcessID's context
		DWORD   module_base_size;        // Size in bytes of module starting at modBaseAddr
		char    module_name[MAX_MODULE_NAME32 + 1];
		DWORD	pe_header_addr;
		int		section_nums;
		section_info_t	section_info[96];

		std::vector<BYTE*> entry_vector;
	};

	struct memory_region_info_t 
	{
		BYTE*	start_addr;
		DWORD	size;
		std::string	owner_name;
		std::string section_name;
		// 		BYTE*	pSectionStart;
// 		DWORD	dwContains;
		DWORD	type;
		DWORD	protect;
		DWORD	alloc_protect;

		std::vector<BYTE*> entry_vector;
		//std::string	strMapAs;
	};

	std::vector<memory_region_info_t>	memory_info_vector_;

	void refresh_memory_map(void);
	bool get_memory_info_by_addr(const void* addr,memory_region_info_t& info)
	{
		refresh_memory_map();
		for each (memory_region_info_t tmp in memory_info_vector_)
		{
			if ((unsigned long)(tmp.start_addr) <= (unsigned long)addr && (unsigned long)addr < (unsigned long)(tmp.start_addr + tmp.size))
			{
				info = tmp;
				return true;
			}
		}

		return false;
	}
};
