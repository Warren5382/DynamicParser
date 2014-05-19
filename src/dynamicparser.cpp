#include "dynamicparser.h"

DynamicParser::DynamicParser()
{
}

int DynamicParser::init(const char* protoname, const char* protopath)
{
	DiskSourceTree sourceTree;
	// find proto file in current directory
	sourceTree.MapPath("", protopath);
	// import proto file
	Importer importer(&sourceTree, NULL);
	importer.Import(protoname);
	return 0;
}

DynamicParser::~DynamicParser()
{
}

int DynamicParser::getfieldvalue(
		google::protobuf::Message *message,
		const google::protobuf::FieldDescriptor * descriptor,
		const google::protobuf::Reflection * reflection,
		int fieldindex)
{
	if (message == NULL || descriptor == NULL || reflection == NULL) {
		std::cout << "ERROR::input is NULL" << std::endl;
		return -1;
	}

	std::cout << "descriptor name is " << descriptor->name() << std::endl;
	std::cout << "message type name is " << message->GetTypeName() << std::endl;
	switch(descriptor->type()) {
		case FieldDescriptor::TYPE_INT64 :
			{
				long long val = reflection->GetInt64(*message, descriptor);
				std::cout << "value is " << val << std::endl;
				break;
			}
		case FieldDescriptor::TYPE_UINT64:
			{
				unsigned long long val = reflection->GetInt64(*message, descriptor);
				std::cout << "value is " << val << std::endl;
				break;
			}
		case FieldDescriptor::TYPE_INT32:
			{
				int val = reflection->GetInt32(*message, descriptor);
				std::cout << "value is " << val << std::endl;
				result->setInt32(val);
				break;
			}
		case FieldDescriptor::TYPE_UINT32:
			{
				unsigned int val = reflection->GetUInt32(*message, descriptor);
				std::cout << "value is " << val << std::endl;
				result->setUint32(val);
				break;
			}
		case FieldDescriptor::TYPE_STRING:
			{
				std::string val = reflection->GetString(*message, descriptor);
				std::cout << "value is " << val << std::endl;
				break;
			}
	}
}

int DynamicParser::doparser() {
	std::queue<LogNode> BFSqueue;
	LogNode now;
	LogNode next;
	BFSqueue.push_back(now);
	while (!BFSqueue.empty()) {
		now = BFSqueue.front();
		// get now node value
		// to be done

		// search next node
		std::vector<DescrpNode> &sondescrpcontainer = DescrpMap[now.name];
		int leefsize = sondescrpcontainer.size();
		for (int index = 0; index < leefsize; index++) {
			DescrpNode &sondescrp = sondescrpcontainer[index];
			next.name = sondescrp.name;
			// because the fathernode must be of message type
			// while basic type like int mush be a leef node
			next.reflection = get<Message *>(now.value)->GetReflection();
			// get nextnode value
			next.value = getfieldvalue(get<Message *>(now.value), sondescrp.field, next.reflection, 0);
			// if the next node's type is Message,
			// this node will be pushed into queue
			if (next.value.type() == typeid(Message *)) {
				BFSqueue.push_back(next);
			}
		}
		BFSqueue.pop();
	}
	return 0;
}
