#include <iostream>
#include <fstream>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/compiler/importer.h>

using namespace std;
using namespace google::protobuf;
using namespace google::protobuf::compiler;

class commonResult {
	public:
		commonResult() {};
		~commonResult() {};
		int setInt32(int val) {
			val_int32 = val;
			return 0;
		};
		int getInt32() {
			return val_int32;
		};
		int setUint32(unsigned int val) {
			val_uint32 = val;
			return 0;
		};
		unsigned int getUint32() {
			return val_uint32;
		};
		int setMessage(Message* message) {
			_message = message;
			return 0;
		}
		Message* getMessage() {
			return _message;
		};
	private:
		int val_int32;
		unsigned int val_uint32;
		Message* _message;
};

int getvalue(google::protobuf::Message *message,
		const google::protobuf::FieldDescriptor * descriptor,
		const google::protobuf::Reflection * reflection,
		commonResult *result)
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
		case FieldDescriptor::TYPE_MESSAGE:
			{
				Message* val = reflection->MutableMessage(message, descriptor);
				std::cout << "is a message" << std::endl;
				result->setMessage(val);
				break;
			}
	}
	return 0;
}

				
int main(int argc, const char *argv[])
{
	DiskSourceTree sourceTree;
	//look up .proto file in current directory
	sourceTree.MapPath("", "./proto");
	Importer importer(&sourceTree, NULL);
	//runtime compile foo.proto
	importer.Import("asp_display.proto");
	//importer.Import("houyi_res_pblog.proto");
	//importer.Import("im_res_pblog.proto");
	//importer.Import("siva_res_pblog.proto");
//importer.Import("wise_req_pblog.proto");
	//                 
	const Descriptor *descriptor = importer.pool()->FindMessageTypeByName("b2log.BaiduLog");
	std::vector<const Descriptor *> descriptorcontainer;
	descriptorcontainer.push_back(descriptor);
	descriptorcontainer.push_back(importer.pool()->FindMessageTypeByName("b2log.AspLogField"));
	descriptorcontainer.push_back(importer.pool()->FindMessageTypeByName("b2log.BaseField"));
	descriptorcontainer.push_back(importer.pool()->FindMessageTypeByName("b2log.LogFieldAddedByAsp"));
	if (descriptor == NULL) {
		std::cout << "find message type by name error!" << std::endl;
		return -1;
	}
	cout << descriptor->DebugString();
	//                          
	// build a dynamic message by "Pair" proto
	DynamicMessageFactory factory;
	const Message *message = factory.GetPrototype(descriptor);
	// create a real instance of "Pair"
	Message *baidulog = message->New();

	std::cout << "begin deserial!" << std::endl;
	// 凡序列化
	fstream in("./data/normalasp.pb", ios::in | ios::binary);
	
	if (!baidulog->ParseFromIstream(&in)) {
		std::cout << "ERROR::failted to parse normalasp.pb" << std::endl;
		return -1;
	}
	in.close();
	cout << baidulog->DebugString();

	// write the "Pair" instance by reflection
	std::vector<std::string> fieldnamecontainer;
	fieldnamecontainer.push_back("asp_log_field");
	fieldnamecontainer.push_back("base_field");
	fieldnamecontainer.push_back("field_added_by_asp");
	fieldnamecontainer.push_back("province_id");

	const FieldDescriptor *field = NULL;
	commonResult result;
	result.setMessage(baidulog);
	for (int i = 0; i < 4; i++) {
		field = descriptorcontainer[i]->FindFieldByName(fieldnamecontainer[i].c_str());
		if (field == NULL) {
			std::cout << "cannot get field " << fieldnamecontainer[i] << std::endl;
			return -1;
		}
		Message* tempmessage = result.getMessage();
		getvalue(tempmessage, field, tempmessage->GetReflection(), &result);
	}

	std::cout << "#######province_id is " << result.getUint32() << std::endl;
	//reflection->SetString(baidulog, field, "my key");
	//field = descroptor->FindFieldByName("value");
	//reflection->SetUInt32(baidulog, field, 1111);


	delete baidulog;

	return 0;
}
