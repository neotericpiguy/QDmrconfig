#ifndef BSONDOC_HPP
#define BSONDOC_HPP

#include <mongoc.h>

#include <ctime>  // date conversion
#include <fstream>
#include <functional>

namespace Mongo {
/** BSONDoc class
 *
 * Used for communication to Mongo
 */
class BSONDoc
{
public:
  /** Default Constructor */
  BSONDoc();
  /** Default Constructor with bson_t* primitive */
  BSONDoc(bson_t* val);
  /** Copy Constructor */
  BSONDoc(const BSONDoc& val);
  /** Copy Constructor for bson_t */
  BSONDoc(const bson_t& val);
  /** Constructor for json string. really weird to use but included*/
  BSONDoc(const std::string& json);
  /** Constructor for initial key bool field*/
  BSONDoc(const std::string& key, bool val);
  /** Constructor for initial key BSONDoc field*/
  BSONDoc(const std::string& key, const BSONDoc& value);
  /** Constructor for initial key const char* field*/
  BSONDoc(const std::string& key, const char* value);
  /** Constructor for initial key const std::string field*/
  BSONDoc(const std::string& key, const std::string& value);
  /** Constructor for initial key const std::vector<BSONDoc> field*/
  BSONDoc(const std::string& key, const std::vector<BSONDoc>& val);
  /** Constructor for initial key int field*/
  BSONDoc(const std::string& key, int val);
  /** Constructor for initial key unsigned int field*/
  BSONDoc(const std::string& key, unsigned int val);
  ~BSONDoc();

  /** Assignment operator for BSONDoc */
  BSONDoc& operator=(const BSONDoc& val);
  /** Assignment operator for bson_t */
  BSONDoc& operator=(const bson_t& val);

  /** Get the oid of a BSONDoc 
   *
   * Kind of weird because oid is a hash but the only good way to represent it
   * is a string
   * */
  std::string getOid() const;

  /** Get a bool at path */
  std::tuple<bool, bool> getBool(const std::string& key) const;
  /** Get a string at path */
  bool getString(std::string& result, const std::string& path) const;
  /** Get a string at path */
  std::string getString(const std::string& path) const;
  /** Get a vector of string at the path */
  std::tuple<std::vector<std::string>, bool> getStringVector(const std::string& path) const;
  /** Get a vector of BSONDoc at the path */
  std::vector<Mongo::BSONDoc> getDocuments(const std::string& path) const;
  /** Get a vector of BSONDoc at the path */
  bool getDocuments(std::vector<Mongo::BSONDoc>& result, const std::string& path) const;

  BSONDoc getDocument(const std::string& path) const;
  bool getDocument(BSONDoc& result, const std::string& path) const;

  template <typename T>
  T get(const std::string& path);

  /** Get the epoch? */
  int64_t getDateTime(const std::string& key) const;
  /** Get Date and time as a string */
  std::string getDateTimeStr(const std::string& key) const;
  /** Get Date in month day, year */
  std::string getDateStr(const std::string& key) const;
  /** Get Date in custom format */
  std::string getDateStr(const std::string& format, const std::string& key) const;

  /** Get a vector of chars that represent a raw binary from key  */
  std::vector<char> getBinary(const std::string& key) const;

  /** Add a child field and BSONDoc */
  BSONDoc& append(const std::string& key, const BSONDoc& val);
  /** Add a child field and const char* */
  BSONDoc& append(const std::string& key, const char* val);
  /** Add a child field and std::string  */
  BSONDoc& append(const std::string& key, const std::string& val);
  /** Add a child field and std::vector<BSONDoc> */
  BSONDoc& append(const std::string& key, const std::vector<BSONDoc>& val);
  /** Add a child field and std::vector<std::string> */
  BSONDoc& append(const std::string& key, const std::vector<std::string>& vector);
  /** Add a child field and std::vector<uint8_t> */
  BSONDoc& append(const std::string& key, const std::vector<uint8_t>& binary);
  /** Add a child field and int */
  BSONDoc& append(const std::string& key, int val);
  /** Add a child field and unsigned */
  BSONDoc& append(const std::string& key, unsigned val);
  /** Add a child field and time_t */
  BSONDoc& append(const std::string& key, time_t time);
  /** Add a child field and bool */
  BSONDoc& append(const std::string& key, bool val);

  /** Add oid to document */
  BSONDoc& appendOid();
  /** Add custom oid to document*/
  BSONDoc& appendOid(const std::string& oid);
  /** Add regex to document */
  BSONDoc& appendRegex(const std::string& key, const std::string& regex, const std::string& options = "");
  /** Add timestamp to document */
  BSONDoc& appendNowUtc(const std::string& key);

  /** Generate a BSONDoc binary document from a file */
  static BSONDoc fromFile(const std::string& filename, const std::string& path);

  /** Display BSONDoc as  a pretty string */
  std::string toString() const;

  /** Display keys in BSONDoc */
  std::vector<std::string> getKeys() const;
  bool isDocument(const std::string& path) const;
  bool isString(const std::string& path) const;

  /** Display the number of child fields */
  unsigned count() const;
  /** returns if BSONDoc has an fields */
  bool empty() const;
  /** check if field exists in document */
  bool hasField(const std::string& key) const;
  bool has(const std::string& key) const;

  int getType(const std::string& path) const;

  /** \deprecated Kind of a dangerous function */
  bson_t* get() const;
  /** Remove all fields and children from Document */
  bool clear();

private:
  bson_t* _doc;
};

}  // namespace Mongo
#endif
