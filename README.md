try to use the reflection of Protobuf to parse log dynamicly

This Parser has three modules.
###
Parser
taking in charge of parse pb log dynamicly.
By excuting a BFS process to get the field value in pb log.

the LogNode is base node struct when BFS a pb log.
struct LogNode {
std::string name;
Descriptor *descriptor;
Message *message;
FieldDescriptor *field;
Reflection *reflection;
}

the name is field name;
descriptor is message descriptor, we could get is by FindMessageTypeByName();
message is the field value of log. 
field is the fielddescriptor, we could get is by descriptor->FindFieldByName();
reflection is the reflection of one message. we could get it by message->GetReflection();

The descriptor is a message's type, while the message is value.

###
Tanslater

###
Outputer
