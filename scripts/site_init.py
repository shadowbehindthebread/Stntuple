import os, re, string, subprocess
#------------------------------------------------------------------------------
def stntuple_codegen_script(source, target, env, for_signature):
#    print "\n>>> mod_codegen called: pwd = "+os.environ['PWD'];
#    print ">>> source = ",len(source),str(source[0])
#    print ">>> target = ",len(target),str(target[0])+'\n'
#    print ">>> env[\'MU2E_BASE_RELEASE\']:"+os.environ['MU2E_BASE_RELEASE']
    
    tmpdir        = os.path.dirname(str(target[0]));
    
    cmd = 'if [ ! -d '+tmpdir+' ] ; then mkdir -p '+tmpdir+'; fi ; ';

    cmd = cmd+'source '+os.environ['MU2E_BASE_RELEASE']+'/Stntuple/scripts/create_print_header_routine.sh '+str(target[0]);

#    print ">>> cmd = %s"%cmd
    return cmd

stntuple_codegen = Builder(generator     = stntuple_codegen_script,
                           single_source = 0,
                           suffix        = '.cc',
                           src_suffix    = '.sh')

#------------------------------------------------------------------------------
def stntuple_gen_rootcint(source, target, env, for_signature):
#    print "\n>>> stntuple_gen_rootcint called:"
#    print ">>> source = ",len(source),str(source[0]), str(source[1])
#    print ">>> target = ",len(target),str(target[0])

    class_include = str(source[1]);
    linkdef       = str(source[0]);

#    print "[stntuple_gen_rootcint] class_include = %s"%class_include
#    print "[stntuple_gen_rootcint] linkdef       = %s"%linkdef
    
    includes =   "-Iinclude -I"+os.environ['ART_INC'     ];
    includes = includes + " -I"+os.environ['BTRK_INC'    ];
    includes = includes + " -I"+os.environ['CETLIB_INC'  ];
    includes = includes + " -I"+os.environ['CETLIB_EXCEPT_INC'];
    includes = includes + " -I"+os.environ['CANVAS_INC'  ];
    includes = includes + " -I"+os.environ['FHICLCPP_INC'];
    includes = includes + " -I"+os.environ['CLHEP_INC'   ];
    includes = includes + " -I"+os.environ['BOOST_INC'   ];

    dict     = str(target[0]);
    tmp_lib_dir = os.path.dirname(dict);
    
    pcm_file = dict.replace(".cxx","_rdict.pcm");

#    print "dict:"+dict + "   pcm_file:"+pcm_file;
    lib_dir = os.environ['MU2E_BASE_RELEASE']+"/lib";
    cmd = 'if [ ! -d '+tmp_lib_dir+' ] ; then mkdir -p '+tmp_lib_dir+'; fi ;';
    cmd = cmd+"rootcint -f "+dict+" -c  -D_CODEGEN_ -DMU2E "+includes+" "+class_include+" "+linkdef+"; ";
    cmd = cmd+'if [ ! -d '+lib_dir+' ] ; then mkdir '+lib_dir+' ; fi ; ';
    cmd = cmd+"mv "+pcm_file+" "+lib_dir+'/.'; 
#    print ">>> cmd = %s"%cmd
    return cmd

stntuple_rootcint = Builder(generator     = stntuple_gen_rootcint,
                            single_source = 0,
                            suffix        = '.o',
                            src_suffix    = '.h')