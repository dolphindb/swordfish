#include "Swordfish.h"
#include <memory>

using oltp::Connection;
using oltp::DB;
using std::make_shared;
using std::shared_ptr;

int main()
{
    // MUST inintialize runtime first
    DolphinDBLib::initializeRuntime();

    // create (or open) a database
    oltp::DBOption option;
    shared_ptr<DB> db = make_shared<DB>("test_db", option);
    Connection conn(*db);
    auto session = conn.getCurrentSession();

    // run .dos on local path
    bool success = session->run("../../demo/orderbookDemo/orderbookDemo.dos");
    if(!success) {
        throw RuntimeException(session->getLastErrorMessage());
    }

    ConstantSP cnt = conn.execute("exec count(*) from outTable");
    std::cout << cnt->getString() << std::endl;
    TableSP res = conn.execute("select top 20 * from outTable");
    std::cout << res->getString() << std::endl;

    DolphinDBLib::finalizeRuntime();
    return 0;
}
