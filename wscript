#! /usr/bin/env python
# encoding: utf-8

# the following two variables are used by the target "waf dist"
VERSION='0.0.1'
APPNAME='cc_test'

top = '.'

import os
import waflib.Configure
waflib.Configure.autoconfig = True

def options(opt):
    opt.load('compiler_c')
    opt.load('gnu_dirs')
    opt.add_option('--with-erlxsl',
                   action='store',
                   default="/usr/local/erlxsl",
                   help='Location of erlxsl')
    opt.add_option('--with-libsablot',
                   action='store',
                   default="/usr/local/lib",
                   help='Location of sablotron library')
    opt.add_option('--with-sablot-headers',
                   action='store',
                   default="/usr/local/include",
                   help='Location of sablotron header file(s)')
    opt.add_option('--enable-64bit',
                   action='store',
                   default=False,
                   help='Build for 64bit architectures')
    opt.add_option('--confcache',
                   dest='confcache',
                   default=0,
                   action='count',
                   help='Use a configuration cache')

def configure(conf):
    conf.load('compiler_c')
    conf.load('gnu_dirs')

    # let the user provide some inputs
    conf.env.prepend_value("INCLUDES", conf.options.with_sablot_headers)
    conf.env.prepend_value("LIBPATH", conf.options.with_libsablot)
    conf.env.prepend_value("INCLUDES", os.path.join(conf.options.with_erlxsl, "priv/include/erlxsl.h"))

    print("INCLUDES %s" % conf.env.INCLUDES)

    if 'darwin' == conf.env.DEST_OS:
        #conf.env.MACBUNDLE = True
        conf.env.append_value('LINKFLAGS', "-bundle")
        conf.env.append_value('LINKFLAGS', '-flat_namespace')

    conf.env.append_value('LINKFLAGS', ['-undefined', 'suppress'])

    if conf.options.enable_64bit:
        # TODO: check conf.env.ARCH and switch for ppc and other architectures....
        conf.env.prepend_value('LINKFLAGS', ['-arch', 'x86_64'])

    print("LINKFLAGS %s" % conf.env.LINKFLAGS)

    # TODO: add conf.check_cfg support
    conf.multicheck(
        {'header_name':'stdio.h'},
        {'header_name':'unistd.h'},
        {'header_name':'stdlib.h'},
        msg       = 'Checking for standard headers',
        features  = "includes",
        mandatory = False
    )
    # TODO: if stdint.h is missing, generate the appropriate checks
    #       by parsing check(fragment="...").decode('utf-8')
    conf.check(header_name='stdint.h',
               auto_add_header_name=True,
               features="includes",
               mandatory=False)
    conf.check(header_name="erlxsl.h",
               auto_add_header_name=True,
               compile_filename="erlxsl_header_check.c",
               features="includes",
               mandatory=True)
    conf.check(header_name="sablot.h",
               auto_add_header_name=True,
               compile_filename="erlxsl_header_check.c",
               features="includes",
               mandatory=True)

    conf.check_inline()
    conf.write_config_header('include/config.h')

    # exclude system libraries, force a particular folder (see strictlib below)
    #conf.check(features='c cprogram strictlib', lib = 'gif', libpath = ['/opt/lib'])

def build(ctx):
    # TODO: can we not do this during conf?
    if 'darwin' == ctx.env.DEST_OS:
        ctx.env['LINKFLAGS_cshlib'] = ctx.env['LINKFLAGS_cxxshlib'] = []
    ctx.shlib(source='src/erlxsl_sablot.c',
        target='erlxsl_sablot',
        includes=["include"],
        use='cstlib',
        vnum="0.1.0")
