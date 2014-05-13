import os

env = Environment(
    ENV = {'PATH' : os.environ['PATH']},
    CPPPATH = ['./src' ],
    CPPFLAGS = ['/EHsc', '/MDd'],
    tools=['default', 'protoc']
    )

proto_files = env.Protoc(
    [],
    "config.proto",
    PROTOCPROTOPATH=['.',r'./src',],
    PROTOCPYTHONOUTDIR='None', # set to None to not generate python
    PROTOCOUTDIR = 'build', # defaults to same directory as .proto
    # PROTOCCPPOUTFLAGS = "dllexport_decl=PROTOCONFIG_EXPORT:", too
)

env.Program('test', ['testmain.cpp', proto_files[0]],
    LIBPATH=[r'C:\wc\protobuf-trunk\vsprojects\Debug'],
    LIBS=['libprotobuf.lib'],
    )
