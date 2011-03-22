#! /usr/bin/env python
# encoding: utf-8
# Thomas Nagy, 2006-2010 (ita)

# the following two variables are used by the target "waf dist"
VERSION='0.0.1'
APPNAME='cc_test'

top = '.'

import waflib.Configure
waflib.Configure.autoconfig = True

def options(opt):
    opt.load('compiler_c')
    opt.load('gnu_dirs')
    opt.add_option('--enable64bit', action='store', default=False, help='Build for 64bit architectures')

def configure(conf):
    conf.load('compiler_c')
    conf.load('gnu_dirs')
    conf.check_cc(fragment="int main() { return 0; }\n")
    conf.check_cc(fragment="int main() { return 0; }\n", execute=True)

    #if conf.opt.enable64bit:
    #    conf.env.prepend_value('LINKFLAGS', ['-arch x86_64'])

    # TODO: add conf.check_cfg support   INCLUDES

    # TODO: if stdint.h is missing, generate the appropriate checks
    #       by parsing check(fragment="...").decode('utf-8')
    conf.check(header_name='stdint.h',
               auto_add_header_name=True,
               mandatory=False)

    conf.check_large_file(mandatory=False)
    conf.check_inline()
    conf.check_library()
    conf.multicheck(
        {'header_name':'stdio.h'},
        {'header_name':'unistd.h'},
        {'header_name':'stdlib.h'},
        msg       = 'Checking for standard headers',
        mandatory = False
    )
    conf.write_config_header('include/config.h')

    # exclude system libraries, force a particular folder (see strictlib below)
    #conf.check(features='c cprogram strictlib', lib = 'gif', libpath = ['/opt/lib'])

def build(bld):
    # TODO: includes should be appended during conf?
    bld.shlib(source='src/erlxsl_sablot.c',
              target='erlxsl_sablot',
              includes=['include'])
