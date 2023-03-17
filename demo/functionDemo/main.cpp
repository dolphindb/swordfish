#include "EmbeddedOLTP.h"

#include <memory>

using std::shared_ptr;
using std::make_shared;

using oltp::DB;
using oltp::Connection;
using oltp::RawTableBuffer;
using oltp::RawTableBufferGenerator;
using oltp::RawTupleBufferReader;
using oltp::RawTupleBufferWriter;
using oltp::RawTableBufferWriter;


int main()
{
    DolphinDBLib::initializeRuntime();

    oltp::DBOption option;
    shared_ptr<DB> db = make_shared<DB>("test_db", option);
    Connection conn(*db);

    //call user define function
    conn.execute(R"(
        def sum5(a, b, c, d, f){
            return a + b + c + d + f;
        }
    )");

    ConstantSP result = conn.execute("sum5([1,2,3], [10,10,10], 5, 0, 10)");
    std::cout <<result->getString() << std::endl;

    //call built-in function
    result = conn.execute("sum3(1 2 3)");
    std::cout <<result->getString() << std::endl;

    DolphinDBLib::finalizeRuntime();
}

