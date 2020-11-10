
variant = variant or lower( 'debug' );

local cc = require 'forge.cc' 
{
    identifier   = 'cc_${platform}_${architecture}';
    platform     = operating_system();
    architecture = 'x86_64';
    bin          = root( ('.build/%s/bin'):format(variant) );
    lib          = root( ('.build/%s/lib'):format(variant) );
    obj          = root( ('.build/%s/obj'):format(variant) );

    include_directories = 
    {
        root();
        root( 'thirdparty/googletest/googletest/include' )
    };

    library_directories = 
    {
        root( ('.build/%s/lib'):format(variant) );
    };

    defines = 
    {
        'COIN_LOG_ENABLED=1';
        'COIN_LOG_VERBOSE=1';
        ('COIN_BUILD_PLATFORM_%s=1'):format( upper(operating_system()) );
        ('COIN_BUILD_VARIANT_%s'):format( upper(variant) );
    };

    assertions                = variant ~= 'shipping';
    debug                     = variant ~= 'shipping';
    debuggable                = variant ~= 'shipping';
    exceptions                = true;
    fast_floating_point       = variant ~= 'debug';
    incremental_linking       = variant == 'debug';
    link_time_code_generation = variant == 'shipping';
    minimal_rebuild           = variant == 'debug';
    objc_arc                  = false;
    optimization              = variant ~= 'debug';
    run_time_checks           = variant == 'debug';
    runtime_library           = variant == 'debug' and 'static_debug' or 'static_release';
    run_time_type_info        = true;
    stack_size                = 1048576;
    standard                  = 'c++17';
    string_pooling            = variant == 'shipping';
    strip                     = false;
    warning_level             = 1;
    warnings_as_errors        = true;
};

if variant == 'debug' then
    table.insert( cc.settings.defines, 'DEBUG' );
else
    table.insert( cc.settings.defines, 'NDEBUG' );
end

buildfile 'thirdparty/googletest.forge';
buildfile 'coin/coin.forge';
buildfile 'main.forge';
