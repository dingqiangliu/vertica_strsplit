/* Copyright (c) 2005 - 2011 Vertica, an HP company -*- C++ -*- */
/*
 * Description: User Defined Transform Function: for each partition, split comma-separated string to multple rows.
 *
 * Copyright Vertica, 2013
 */
#include "Vertica.h"
#include <sstream>

using namespace Vertica;
using namespace std;

#define DEFAULT_separator ","


class StrSplit : public TransformFunction
{
    virtual void processPartition(ServerInterface &srvInterface,
                                  PartitionReader &input_reader,
                                  PartitionWriter &output_writer)
    {
        if (input_reader.getNumCols() != 1)
            vt_report_error(0, "Function only accepts 1 argument, but %zu provided", input_reader.getNumCols());

        ParamReader paramReader = srvInterface.getParamReader();

        std::string separator = DEFAULT_separator;
        if (paramReader.containsParameter("separator")){
            separator = paramReader.getStringRef("separator").str();
        }

        do {
            const VString &sentence = input_reader.getStringRef(0);
            const char delim = separator.c_str()[0];

            // If input string is NULL, then output is NULL as well
            if (sentence.isNull()) {
                VString &word = output_writer.getStringRef(0);
                word.setNull();

                output_writer.next();
            }
            else {
                // Otherwise, let's tokenize the string and output the words
                std::string tmp = sentence.str();
                istringstream ss(tmp);
                std::string token;

                while (getline(ss, token, delim)) {
                        VString &word = output_writer.getStringRef(0);
                        word.copy(token);

                        output_writer.next();
                }

            }
        } while (input_reader.next());
     }
};

class StrSplitFactory : public TransformFunctionFactory
{
    virtual void getPrototype(ServerInterface &srvInterface, ColumnTypes &argTypes, ColumnTypes &returnType)
    {
        // get parameters
        ParamReader paramReader = srvInterface.getParamReader();

        argTypes.addAny();
        
        // Note: need not add any type to returnType. empty returnType means any columns and types!
    }

    virtual void getReturnType(ServerInterface &srvInterface,
                               const SizedColumnTypes &input_types,
                               SizedColumnTypes &output_types)
    {
        if (input_types.getColumnCount() != 1)
            vt_report_error(0, "Function only accepts 1 argument, but %zu provided", input_types.getColumnCount());

        int columncount = input_types.getColumnCount();
        for(int i=0; i<columncount; i++) {
            output_types.addVarchar(input_types.getColumnType(i).getStringLength(), input_types.getColumnName(i));
        }
    }

    // Defines the parameters for this UDSF. Works similarly to defining
    // arguments and return types.
    virtual void getParameterType(ServerInterface &srvInterface,
                                  SizedColumnTypes &parameterTypes) {
        //parameter: separator string for concatenating, default value is ','.
        parameterTypes.addVarchar(1, "separator");
    }


    virtual TransformFunction *createTransformFunction(ServerInterface &srvInterface)
    { return vt_createFuncObj(srvInterface.allocator, StrSplit); }

};

RegisterFactory(StrSplitFactory);
