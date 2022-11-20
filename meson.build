project('DSPatcher', 'cpp',
    default_options: [
        'cpp_std=c++11',
        'warning_level=3',
        'werror=true'
    ]
)

# Add formatting

format_first = []
if find_program('clang-format', required: false).found()
    if host_machine.system() == 'windows'
        format_command = meson.current_source_dir() + '/scripts/clang-format.bat'
    else
        format_command = ['bash', meson.current_source_dir() + '/scripts/clang-format.sh']
    endif

    format_first = custom_target(
        output: 'formatting',
        command: format_command,
        build_by_default: true
    )
endif

# Add static code analysis

if find_program('cppcheck', required: false).found()
    if host_machine.system() == 'windows'
        extra_cppcheck_args = ['-D_WIN32']
    else
        extra_cppcheck_args = []
    endif

    custom_target(
        input: format_first,
        output: 'static code analysis',
        command: [
            'cppcheck',

            '--error-exitcode=1',
            '--enable=all',
            '--inconclusive',
            '--inline-suppr',
            '--force',
            '--quiet',
            '--suppress=missingIncludeSystem',

            '-i', meson.current_source_dir() + '/build',
            '-i', meson.current_source_dir() + '/builddir',
            '-i', meson.current_source_dir() + '/DSPatchables',

            extra_cppcheck_args,

            meson.current_source_dir()
        ],
        build_by_default: true
    )
endif
