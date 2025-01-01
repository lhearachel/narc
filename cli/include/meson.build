cli_version_h = custom_target(
    'cli_version_h',
    output: 'version.h',
    command: [version_sh, version, '@OUTPUT@'],
)

cli_includes = include_directories('.')
