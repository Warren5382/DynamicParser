#ifndef DYNAMIC_PARSER_H
#define DYNAMIC_PARSER_H

class DynamicParser {
public:
	// construct function
	DynamicParser();
	~DynamicParser();
	// the main funciton to do parse
	int doparser();
private:
	// getvalue from field
	int getfieldvalue(
			google::protobuf::Message *message,
			const google::protobuf::FieldDescriptor *descriptor,
			const google::protobuf::Reflection *reflection,
			int fieldindex
			);
}




