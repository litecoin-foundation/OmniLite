#ifndef BITCOIN_OMNICORE_DBTXLIST_H
#define BITCOIN_OMNICORE_DBTXLIST_H

#include <omnicore/dbbase.h>

#include <fs.h>
#include <uint256.h>

#include <stdint.h>

#include <set>
#include <string>

/** LevelDB based storage for transactions, with txid as key and validity bit, and other data as value.
 */
class CMPTxList : public CDBBase
{
public:
    CMPTxList(const fs::path& path, bool fWipe);
    virtual ~CMPTxList();

    void recordTX(const uint256& txid, bool fValid, int nBlock, unsigned int type, uint64_t nValue);
    void recordPaymentTX(const uint256& txid, bool fValid, int nBlock, unsigned int vout, unsigned int propertyId, uint64_t nValue, std::string buyer, std::string seller);
    /** Records a "send all" sub record. */
    void recordSendAllSubRecord(const uint256& txid, int subRecordNumber, uint32_t propertyId, int64_t nvalue);

    std::string getKeyValue(std::string key);
    /** Returns the number of sub records. */
    int getNumberOfSubRecords(const uint256& txid);
    bool getPurchaseDetails(const uint256 txid, int purchaseNumber, std::string* buyer, std::string* seller, uint64_t* vout, uint64_t *propertyId, uint64_t* nValue);
    /** Retrieves details about a "send all" record. */
    bool getSendAllDetails(const uint256& txid, int subSend, uint32_t& propertyId, int64_t& amount);
    int getMPTransactionCountTotal();
    int getMPTransactionCountBlock(int block);
    /** Returns a list of all Omni transactions in the given block range. */
    int GetOmniTxsInBlockRange(int blockFirst, int blockLast, std::set<uint256>& retTxs);

    int getDBVersion();
    int setDBVersion();

    bool exists(const uint256& txid);
    bool getTX(const uint256& txid, std::string& value);
    bool getValidMPTX(const uint256& txid, int* block = nullptr, unsigned int* type = nullptr, uint64_t* nAmended = nullptr);

    void LoadAlerts(int blockHeight);
    void LoadActivations(int blockHeight);
    bool LoadFreezeState(int blockHeight);
    bool CheckForFreezeTxs(int blockHeight);

    void printStats();
    void printAll();

    bool isMPinBlockRange(int, int, bool);
};

namespace mastercore
{
    //! LevelDB based storage for transactions, with txid as key and validity bit, and other data as value
    extern CMPTxList* pDbTransactionList;
}


#endif // BITCOIN_OMNICORE_DBTXLIST_H
