def init(celery_app, **global_decorators):
    authorized_task = global_decorators['authorized_task']

    #@authorized_task
    @celery_app.task
    def run_checkpoint1(x: int, y: int):
        program_output = None
        try:
            import os
            import sys
            from tasks.packs.soicmc.resources import sys_utils
            
            bin_name = "entrypoint.sh"
            bin_path = sys_utils.default_bin_dir + "/" + "checkpoint1" + "/"
            
            program_stdout, program_stderr = sys_utils.run_command(f"./{bin_name}", cwd=bin_path, log_stdout=True, log_stderr=True, timeout_sec=7)
            program_output = [program_stdout, program_stderr]
        except Exception as exc:
            program_output = f"Failed to execute task. Error: {str(exc)}"
        
        return program_output
    
    #@authorized_task
    @celery_app.task
    def run_checkpoint2(x: int, y: int):
        program_output = None
        try:
            import os
            import sys
            from tasks.packs.soicmc.resources import sys_utils
            
            bin_name = "entrypoint.sh"
            bin_path = sys_utils.default_bin_dir + "/" + "checkpoint2" + "/"
            
            program_stdout, program_stderr = sys_utils.run_command(f"./{bin_name}", cwd=bin_path, log_stdout=True, log_stderr=True, timeout_sec=10)
            program_output = [program_stdout, program_stderr]
        except Exception as exc:
            program_output = f"Failed to execute task. Error: {str(exc)}"
        
        return program_output
    
    #@authorized_task
    @celery_app.task
    def run_checkpoint3(x: float, y: float):
        program_output = None
        try:
            import os
            import sys
            from tasks.packs.soicmc.resources import sys_utils
            
            bin_name = "entrypoint.sh"
            bin_path = sys_utils.default_bin_dir + "/" + "checkpoint3" + "/"
            
            program_stdout, program_stderr = sys_utils.run_command(f"./{bin_name}", cwd=bin_path, log_stdout=True, log_stderr=True, timeout_sec=7)
            program_output = [program_stdout, program_stderr]
        except Exception as exc:
            program_output = f"Failed to execute task. Error: {str(exc)}"
        
        return program_output