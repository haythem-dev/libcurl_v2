#!/usr/bin/env python3
"""
libcurl Build Guide & Automation Web Application
A comprehensive Flask application for building libcurl from source with real-time progress tracking.
Supports both Unix/Linux and Windows environments.
"""

import os
import sys
import json
import time
import subprocess
import threading
import tempfile
import platform
from flask import Flask, render_template, jsonify, request, send_file, Response, abort
from werkzeug.utils import secure_filename

app = Flask(__name__)

# Global variables for build status tracking
build_status = {
    'running': False,
    'stage': 'idle',
    'progress': 0,
    'log': [],
    'error': None,
    'start_time': None,
    'end_time': None
}

build_lock = threading.Lock()

def detect_system_info():
    """Detect system information and available tools."""
    info = {
        'os': platform.system(),
        'os_version': platform.release(),
        'platform': platform.platform(),
        'architecture': platform.machine(),
        'python_version': platform.python_version(),
        'tools': {},
        'environment': {}
    }
    
    # Detect available tools
    tools_to_check = [
        'gcc', 'clang', 'make', 'cmake', 'git', 'wget', 'curl', 
        'openssl', 'pkg-config', 'autoconf', 'automake', 'libtool'
    ]
    
    # Windows-specific tools
    if info['os'] == 'Windows':
        tools_to_check.extend(['cl', 'msbuild', '7z', 'tar', 'choco', 'winget'])
    
    for tool in tools_to_check:
        try:
            if info['os'] == 'Windows':
                result = subprocess.run(['where', tool], 
                                      capture_output=True, text=True, timeout=5)
            else:
                result = subprocess.run(['which', tool], 
                                      capture_output=True, text=True, timeout=5)
            info['tools'][tool] = result.returncode == 0
        except (subprocess.TimeoutExpired, FileNotFoundError):
            info['tools'][tool] = False
    
    # Get environment variables
    env_vars = ['PATH', 'CC', 'CXX', 'CFLAGS', 'CXXFLAGS', 'LDFLAGS', 'PKG_CONFIG_PATH']
    if info['os'] == 'Windows':
        env_vars.extend(['VCINSTALLDIR', 'WindowsSDKDir', 'CMAKE_GENERATOR'])
    
    for var in env_vars:
        info['environment'][var] = os.environ.get(var, '')
    
    return info

def log_message(message, level='info'):
    """Add a message to the build log."""
    timestamp = time.strftime('%H:%M:%S')
    log_entry = {
        'timestamp': timestamp,
        'level': level,
        'message': message
    }
    build_status['log'].append(log_entry)
    print(f"[{timestamp}] {level.upper()}: {message}")

def run_command(command, cwd=None, timeout=300):
    """Execute a command and capture output."""
    try:
        log_message(f"Executing: {command}")
        
        # Determine shell based on OS
        use_shell = True
        if platform.system() == 'Windows':
            # Use cmd.exe for Windows
            if not command.startswith('cmd'):
                command = f'cmd /c {command}'
        
        process = subprocess.Popen(
            command,
            shell=use_shell,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            cwd=cwd,
            bufsize=1,
            universal_newlines=True
        )
        
        output_lines = []
        while True:
            output = process.stdout.readline()
            if output == '' and process.poll() is not None:
                break
            if output:
                line = output.strip()
                output_lines.append(line)
                log_message(line, 'output')
        
        process.wait(timeout=timeout)
        
        if process.returncode == 0:
            log_message(f"Command completed successfully", 'success')
            return True, '\n'.join(output_lines)
        else:
            log_message(f"Command failed with return code {process.returncode}", 'error')
            return False, '\n'.join(output_lines)
            
    except subprocess.TimeoutExpired:
        log_message(f"Command timed out after {timeout} seconds", 'error')
        return False, "Command timed out"
    except Exception as e:
        log_message(f"Command execution failed: {str(e)}", 'error')
        return False, str(e)

def build_worker(config):
    """Background worker for the build process."""
    global build_status
    
    try:
        with build_lock:
            build_status['running'] = True
            build_status['stage'] = 'preparing'
            build_status['progress'] = 0
            build_status['log'] = []
            build_status['error'] = None
            build_status['start_time'] = time.time()
        
        # Determine script extension based on OS
        script_ext = '.bat' if platform.system() == 'Windows' else '.sh'
        script_dir = os.path.join(os.path.dirname(__file__), 'scripts')
        
        # Stage 1: Install dependencies
        if config.get('install_dependencies', False):
            build_status['stage'] = 'dependencies'
            build_status['progress'] = 10
            
            script_path = os.path.join(script_dir, f'install_dependencies{script_ext}')
            if os.path.exists(script_path):
                success, output = run_command(script_path)
                if not success:
                    raise Exception(f"Dependencies installation failed: {output}")
            else:
                log_message(f"Dependencies script not found: {script_path}", 'warning')
        
        # Stage 2: Download source
        build_status['stage'] = 'downloading'
        build_status['progress'] = 30
        
        download_script = os.path.join(script_dir, f'download_libcurl{script_ext}')
        if os.path.exists(download_script):
            # Set environment variables for configuration
            env = os.environ.copy()
            if config.get('curl_version'):
                env['CURL_VERSION'] = config['curl_version']
            if config.get('download_dir'):
                env['DOWNLOAD_DIR'] = config['download_dir']
            
            success, output = run_command(download_script)
            if not success:
                raise Exception(f"Source download failed: {output}")
        else:
            raise Exception(f"Download script not found: {download_script}")
        
        # Stage 3: Build
        build_status['stage'] = 'building'
        build_status['progress'] = 60
        
        build_script = os.path.join(script_dir, f'build_libcurl{script_ext}')
        if os.path.exists(build_script):
            # Set build configuration environment variables
            env = os.environ.copy()
            for key, value in config.items():
                if key.startswith('BUILD_') or key.startswith('ENABLE_'):
                    env[key] = str(value)
            
            success, output = run_command(build_script)
            if not success:
                raise Exception(f"Build failed: {output}")
        else:
            raise Exception(f"Build script not found: {build_script}")
        
        # Stage 4: Verify
        if config.get('verify_build', True):
            build_status['stage'] = 'verifying'
            build_status['progress'] = 90
            
            verify_script = os.path.join(script_dir, f'verify_build{script_ext}')
            if os.path.exists(verify_script):
                success, output = run_command(verify_script)
                if not success:
                    log_message(f"Verification failed: {output}", 'warning')
            else:
                log_message(f"Verification script not found: {verify_script}", 'warning')
        
        # Complete
        build_status['stage'] = 'complete'
        build_status['progress'] = 100
        log_message("Build completed successfully!", 'success')
        
    except Exception as e:
        build_status['error'] = str(e)
        build_status['stage'] = 'error'
        log_message(f"Build failed: {str(e)}", 'error')
    
    finally:
        build_status['running'] = False
        build_status['end_time'] = time.time()

@app.route('/')
def home():
    """Home page with project overview."""
    if os.path.exists('templates/guide.html'):
        return render_template('guide.html')
    elif os.path.exists('index.html'):
        return send_file('index.html')
    else:
        return jsonify({'error': 'No homepage template found'}), 404

@app.route('/guide')
def guide():
    """Build guide interface."""
    return render_template('guide.html')

@app.route('/api/system-info')
def api_system_info():
    """Get system information and tool availability."""
    return jsonify(detect_system_info())

@app.route('/api/start-build', methods=['POST'])
def api_start_build():
    """Start the build process with given configuration."""
    if build_status['running']:
        return jsonify({'error': 'Build already in progress'}), 400
    
    config = request.get_json() or {}
    
    # Start build in background thread
    thread = threading.Thread(target=build_worker, args=(config,))
    thread.daemon = True
    thread.start()
    
    return jsonify({'status': 'started'})

@app.route('/api/build-status')
def api_build_status():
    """Server-sent events stream for build progress."""
    def event_stream():
        last_log_count = 0
        while True:
            # Send current status
            status_data = {
                'running': build_status['running'],
                'stage': build_status['stage'],
                'progress': build_status['progress'],
                'error': build_status['error']
            }
            
            # Send new log entries
            current_log_count = len(build_status['log'])
            if current_log_count > last_log_count:
                new_logs = build_status['log'][last_log_count:]
                status_data['new_logs'] = new_logs
                last_log_count = current_log_count
            
            yield f"data: {json.dumps(status_data)}\n\n"
            
            # Exit if build is complete
            if not build_status['running'] and build_status['stage'] in ['complete', 'error']:
                break
                
            time.sleep(1)
    
    return Response(event_stream(), mimetype='text/event-stream')

@app.route('/api/cleanup', methods=['POST'])
def api_cleanup():
    """Clean up build artifacts."""
    if build_status['running']:
        return jsonify({'error': 'Cannot cleanup while build is running'}), 400
    
    try:
        script_ext = '.bat' if platform.system() == 'Windows' else '.sh'
        script_dir = os.path.join(os.path.dirname(__file__), 'scripts')
        cleanup_script = os.path.join(script_dir, f'cleanup{script_ext}')
        
        if os.path.exists(cleanup_script):
            success, output = run_command(cleanup_script)
            if success:
                return jsonify({'status': 'success', 'output': output})
            else:
                return jsonify({'status': 'error', 'output': output}), 500
        else:
            return jsonify({'error': f'Cleanup script not found: {cleanup_script}'}), 404
            
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/download/<script_name>')
def download_script(script_name):
    """Download individual build scripts."""
    # Security check
    if '..' in script_name or '/' in script_name or '\\' in script_name:
        abort(400)
    
    script_path = os.path.join('scripts', secure_filename(script_name))
    
    if not os.path.exists(script_path):
        abort(404)
    
    return send_file(script_path, as_attachment=True)

@app.route('/api/logs')
def api_logs():
    """Get current build logs."""
    return jsonify({
        'logs': build_status['log'],
        'running': build_status['running'],
        'stage': build_status['stage'],
        'progress': build_status['progress']
    })

if __name__ == '__main__':
    print("libcurl Build Guide & Automation")
    print("================================")
    print(f"Platform: {platform.system()} {platform.release()}")
    print(f"Python: {platform.python_version()}")
    print()
    print("Starting Flask application...")
    print("Open your browser to: http://localhost:5000")
    print("Press Ctrl+C to stop")
    print()
    
    # Run Flask app
    app.run(host='0.0.0.0', port=5000, debug=True, threaded=True)