#include "BSONDoc.hpp"

namespace Mongo {
BSONDoc::BSONDoc() :
    _doc(bson_new())
{
}

BSONDoc::BSONDoc(bson_t* val) :
    _doc(val)
{
}

BSONDoc::BSONDoc(const bson_t& val) :
    _doc(bson_copy(&val))
{
}

BSONDoc::BSONDoc(const std::string& json) :
    _doc(nullptr)
{
  bson_error_t error;

  const unsigned char* data = reinterpret_cast<const unsigned char*>(json.c_str());

  _doc = bson_new_from_json(data, -1, &error);

  if (!_doc)
  {
    printf("Error: %s\n", error.message);
    printf("Error: %s\n", json.c_str());
  }
}

bool BSONDoc::isValid(const std::string& json)
{
  bson_error_t error;

  const unsigned char* data = reinterpret_cast<const unsigned char*>(json.c_str());

  bson_t* temp = bson_new_from_json(data, -1, &error);

  if (temp)
    bson_destroy(temp);

  return static_cast<bool>(temp);
}

BSONDoc::BSONDoc(const std::string& key, const std::string& value) :
    _doc(bson_new())
{
  append(key, value);
}

BSONDoc::BSONDoc(const std::string& key, const char* value) :
    _doc(bson_new())
{
  append(key, value);
}

BSONDoc::BSONDoc(const std::string& key, const BSONDoc& value) :
    _doc(bson_new())
{
  append(key, value);
}

BSONDoc::BSONDoc(const std::string& key, unsigned int val) :
    BSONDoc(key, static_cast<int>(val))
{
}
BSONDoc::BSONDoc(const std::string& key, int value) :
    _doc(bson_new())
{
  append(key, value);
}

BSONDoc::BSONDoc(const std::string& key, bool value) :
    _doc(bson_new())
{
  append(key, value);
}

BSONDoc::BSONDoc(const std::string& key, const std::vector<BSONDoc>& value) :
    _doc(bson_new())
{
  append(key, value);
}

BSONDoc::BSONDoc(const BSONDoc& val) :
    _doc(bson_copy(val._doc))
{
  //  std::cout <<"BSONDoc& val" << std::endl;
}

BSONDoc::~BSONDoc()
{
  bson_destroy(_doc);
}

BSONDoc& BSONDoc::operator=(const BSONDoc& val)
{
  bson_destroy(_doc);
  _doc = bson_copy(val._doc);
  return *this;
}

BSONDoc& BSONDoc::operator=(const bson_t& val)
{
  bson_destroy(_doc);
  _doc = bson_copy(&val);
  return *this;
}

bson_t* BSONDoc::get() const
{
  return _doc;
}

bool BSONDoc::clear()
{
  bson_destroy(_doc);
  _doc = bson_new();
  return true;
}

std::string BSONDoc::toString() const
{
  char* str;
  str = bson_as_json(_doc, NULL);
  std::string result(str);
  bson_free(str);

  return result;
}

std::string BSONDoc::getOid() const
{
  bson_iter_t iter;
  if (bson_iter_init(&iter, _doc))
  {
    while (bson_iter_next(&iter))
    {
      if (bson_iter_type(&iter) == BSON_TYPE_OID)
      {
        char str[25];
        const bson_oid_t* oid = bson_iter_oid(&iter);
        bson_oid_to_string(oid, str);

        return std::string(str);
      }
    }
  }
  return "";
}

std::string BSONDoc::getString(const std::string& path) const
{
  std::string temp;
  getString(temp, path);
  return temp;
}

bool BSONDoc::getString(std::string& result, const std::string& path) const
{
  bson_iter_t iter;
  bson_iter_t baz;

  if (bson_iter_init(&iter, _doc) && bson_iter_find_descendant(&iter, path.c_str(), &baz))
  {
    switch (bson_iter_type(&baz))
    {
      case BSON_TYPE_OID:
      {
        char str[25];
        const bson_oid_t* oid = bson_iter_oid(&baz);
        bson_oid_to_string(oid, str);
        result = std::string(str);
      }
      break;
      case BSON_TYPE_UTF8:
        result = std::string(bson_iter_utf8(&baz, nullptr));
        break;
      default:
        result = "";
    }
    return true;
  }
  return false;
}

//std::vector<std::string> BSONDoc::getStringVector(const std::string& path) const
//{
//  std::vector<std::string> result;
//  getStringVector(result, path);
//  return result;
//}
//
//bool BSONDoc::getStringVector(std::vector<std::string>& result, const std::string& path) const
//{
//  bson_iter_t iter;
//  bson_iter_t sub_iter;
//
//  if (bson_iter_init_find(&iter, _doc, path.c_str()))
//  {
//    if (BSON_ITER_HOLDS_ARRAY(&iter))
//    {
//      if (bson_iter_recurse(&iter, &sub_iter))
//      {
//        while (bson_iter_next(&sub_iter))
//          result.push_back(std::string(bson_iter_utf8(&sub_iter, nullptr)));
//      }
//    }
//  }
//
//  return true;
//}

std::tuple<std::vector<std::string>, bool> BSONDoc::getStringVector(const std::string& path) const
{
  bson_iter_t iter;
  bson_iter_t sub_iter;

  std::vector<std::string> result;

  if (bson_iter_init_find(&iter, _doc, path.c_str()))
  {
    if (BSON_ITER_HOLDS_ARRAY(&iter))
    {
      if (bson_iter_recurse(&iter, &sub_iter))
      {
        while (bson_iter_next(&sub_iter))
          result.push_back(std::string(bson_iter_utf8(&sub_iter, nullptr)));
      }
    }
  }

  return {result, true};
}

BSONDoc& BSONDoc::append(const std::string& key, const std::vector<std::string>& vector)
{
  bson_t child;
  char buf[256];
  size_t keylen;
  const char* field;

  bson_append_array_begin(_doc, key.c_str(), key.length(), &child);
  for (unsigned int i = 0; i < vector.size(); i++)
  {
    keylen = bson_uint32_to_string(i, &field, buf, sizeof buf);
    bson_append_utf8(&child, field, static_cast<int>(keylen), vector[i].c_str(), -1);
  }
  bson_append_array_end(_doc, &child);
  return *this;
}

BSONDoc& BSONDoc::append(const std::string& key, const std::vector<BSONDoc>& val)
{
  bson_t child;
  char buf[256];
  size_t keylen;
  const char* field;
  bson_append_array_begin(_doc, key.c_str(), key.length(), &child);
  for (unsigned int i = 0; i < val.size(); i++)
  {
    keylen = bson_uint32_to_string(i, &field, buf, sizeof buf);
    bson_append_document(&child, field, static_cast<int>(keylen), val[i]._doc);
  }
  bson_append_array_end(_doc, &child);

  return *this;
}

BSONDoc& BSONDoc::append(const std::string& key, const char* value)
{
  return append(key, std::string(value));
}

BSONDoc& BSONDoc::append(const std::string& key, const std::string& value)
{
  //http://mongoc.org/libbson/1.1.0/bson_oid_t.html
  if (key == "_id")
    appendOid(value);
  else
    bson_append_utf8(_doc, key.c_str(), key.length(), value.c_str(), value.length());
  return *this;
}

BSONDoc& BSONDoc::appendOid()
{
  return appendOid("");
}

BSONDoc& BSONDoc::appendOid(const std::string& oid)
{
  //  http://mongoc.org/libbson/1.1.2/bson_oid_t.html
  const std::string key = "_id";
  bson_oid_t boid;
  if (oid != "" && bson_oid_is_valid(oid.c_str(), oid.length()))
    bson_oid_init_from_string(&boid, oid.c_str());
  else
    bson_oid_init(&boid, NULL);

  bson_append_oid(_doc, key.c_str(), key.length(), &boid);
  return *this;
}

BSONDoc& BSONDoc::append(const std::string& key, const BSONDoc& val)
{
  //http://mongoc.org/libbson/1.1.2/bson_append_value.html
  bson_append_document(_doc, key.c_str(), key.length(), val._doc);
  return *this;
}

BSONDoc& BSONDoc::append(const std::string& key, unsigned val)
{
  append(key, static_cast<int>(val));
  return *this;
}

BSONDoc& BSONDoc::append(const std::string& key, int val)
{
  bson_append_int64(_doc, key.c_str(), key.length(), val);
  return *this;
}

std::tuple<bool, bool> BSONDoc::getBool(const std::string& key) const
{
  bson_iter_t iter;
  bson_iter_t baz;

  //  if (bson_iter_init_find (&iter, _doc, key.c_str()))
  if (bson_iter_init(&iter, _doc) && bson_iter_find_descendant(&iter, key.c_str(), &baz))
  {
    if (BSON_ITER_HOLDS_BOOL(&baz))
      return {bson_iter_bool(&baz), true};
  }
  return {false, false};
}

BSONDoc& BSONDoc::append(const std::string& key, bool val)
{
  bson_append_bool(_doc, key.c_str(), key.length(), val);
  return *this;
}

BSONDoc& BSONDoc::appendRegex(const std::string& key, const std::string& regex, const std::string& options)
{
  //http://mongoc.org/libbson/1.0.0/bson_append_regex.html
  bson_append_regex(_doc, key.c_str(), key.length(), regex.c_str(), options.c_str());
  return *this;
}

unsigned BSONDoc::count() const
{
  return bson_count_keys(_doc);
}

bool BSONDoc::empty() const
{
  return count() == 0;
}

bool BSONDoc::has(const std::string& key) const
{
  return bson_has_field(_doc, key.c_str());
}

BSONDoc& BSONDoc::append(const std::string& key, time_t time)
{
  bson_append_date_time(_doc, key.c_str(), key.length(), time * 1000);
  return *this;
}

BSONDoc& BSONDoc::appendNowUtc(const std::string& key)
{
  bson_append_now_utc(_doc, key.c_str(), key.length());
  return *this;
}

int64_t BSONDoc::getDateTime(const std::string& key) const
{
  bson_iter_t iter;
  bson_iter_t baz;

  if (bson_iter_init(&iter, _doc) && bson_iter_find_descendant(&iter, key.c_str(), &baz))
  {
    if (BSON_ITER_HOLDS_DATE_TIME(&baz))
      return bson_iter_date_time(&baz) / 1000;
    else
      printf("Not a date time");
  }

  return 0;
}

std::string BSONDoc::getDateTimeStr(const std::string& key) const
{
  time_t time = getDateTime(key);
  auto temp = std::string(std::asctime(std::localtime(&time)));
  return temp.substr(0, temp.length() - 1);
}

std::string BSONDoc::getDateStr(const std::string& format, const std::string& key) const
{
  time_t time = getDateTime(key);
  char buffer[80];

  std::strftime(buffer, 80, format.c_str(), std::localtime(&time));

  auto temp = std::string(buffer);
  return temp;
}

std::string BSONDoc::getDateStr(const std::string& key) const
{
  return getDateStr("%b %d, %Y", key);
}

BSONDoc& BSONDoc::append(const std::string& key, const std::vector<uint8_t>& binary)
{
  //http://mongoc.org/libbson/1.2.2/bson_subtype_t.html
  bson_append_binary(_doc, key.c_str(), key.length(), BSON_SUBTYPE_BINARY, binary.data(), binary.size());
  return *this;
}

std::vector<char> BSONDoc::getBinary(const std::string& key) const
{
  //  printf("Getting binary\n");
  bson_iter_t iter;
  bson_iter_t baz;

  std::vector<char> result;
  if (bson_iter_init(&iter, _doc) && bson_iter_find_descendant(&iter, key.c_str(), &baz))
  {
    if (BSON_ITER_HOLDS_BINARY(&baz))
    {
      uint32_t length;
      const uint8_t* binary;
      bson_iter_binary(&baz, NULL, &length, &binary);
      return std::vector<char>(binary, binary + length);
    }
  }
  return result;
}

BSONDoc BSONDoc::fromFile(const std::string& filename, const std::string& path)
{
  BSONDoc result;

  std::ifstream testFile(path, std::ios::binary);
  std::vector<uint8_t> fileContents;

  //  fileContents.reserve(fileSize);
  fileContents.assign(std::istreambuf_iterator<char>(testFile),
                      std::istreambuf_iterator<char>());

  result.append("filename", filename);
  result.append("file", fileContents);

  return result;
}

std::vector<std::string> BSONDoc::getKeys() const
{
  bson_iter_t iter;
  std::vector<std::string> result;
  if (bson_iter_init(&iter, _doc))
  {
    while (bson_iter_next(&iter))
    {
      result.push_back(bson_iter_key(&iter));
    }
  }
  return result;
}

bool BSONDoc::isDocument(const std::string& path) const
{
  bson_iter_t iter;
  if (bson_iter_init_find(&iter, _doc, path.c_str()))
  {
    return BSON_ITER_HOLDS_DOCUMENT(&iter);
  }
  return false;
}

bool BSONDoc::isString(const std::string& path) const
{
  bson_iter_t iter;
  if (bson_iter_init_find(&iter, _doc, path.c_str()))
  {
    return BSON_ITER_HOLDS_UTF8(&iter);
  }
  return false;
}

template <>
std::string BSONDoc::get<std::string>(const std::string& path) const
{
  bson_iter_t iter;
  bson_iter_t baz;
  if (bson_iter_init(&iter, _doc) && bson_iter_find_descendant(&iter, path.c_str(), &baz))
  {
    if (BSON_ITER_HOLDS_UTF8(&baz))
    {
      return std::string(bson_iter_utf8(&baz, nullptr));
    }
  }
  return "";
}

template <>
int32_t BSONDoc::get<int32_t>(const std::string& path) const
{
  bson_iter_t iter;
  bson_iter_t baz;
  if (bson_iter_init(&iter, _doc) && bson_iter_find_descendant(&iter, path.c_str(), &baz))
  {
    if (BSON_ITER_HOLDS_INT32(&baz))
    {
      return int32_t(bson_iter_int32(&baz));
    }
    printf("no int");
  }
  return 0;
}

template <>
BSONDoc BSONDoc::get<BSONDoc>(const std::string& path) const
{
  bson_iter_t iter;
  bson_iter_t baz;

  if (bson_iter_init(&iter, _doc) && bson_iter_find_descendant(&iter, path.c_str(), &baz))
  {
    if (BSON_ITER_HOLDS_DOCUMENT(&baz))
    {
      const uint8_t* document;
      uint32_t document_len;
      bson_t temp;
      bson_iter_document(&baz, &document_len, &document);
      if (bson_init_static(&temp, document, document_len))
      {
        BSONDoc result(&temp);
        return result;
      }
    }
  }
  static BSONDoc empty;
  return empty;
}

template <>
std::vector<BSONDoc> BSONDoc::get<std::vector<BSONDoc>>(const std::string& path) const
{
  bson_iter_t iter;
  bson_iter_t baz;
  std::vector<BSONDoc> result;

  if (!bson_iter_init(&iter, _doc) || !bson_iter_find_descendant(&iter, path.c_str(), &baz))
  {
    printf("Path not found: \"%s\"\n", path.c_str());
    printf("\"%s\"\n", toString().c_str());
    return result;
  }

  if (!BSON_ITER_HOLDS_ARRAY(&baz))
  {
    printf("Path not an ARRAY: \"%s\"\n", path.c_str());
    return result;
  }

  uint32_t array_len = 0;
  const uint8_t* array = nullptr;
  bson_iter_array(&baz, &array_len, &array);

  BSONDoc temp(bson_new_from_data(array, array_len));
  bson_iter_t sub_iter;
  if (!(bson_iter_init(&sub_iter, temp.get()) && bson_iter_find_descendant(&sub_iter, "0", &baz)))
  {
    printf("Path not found: \"0\"\n");
    printf("\"%s\"\n", temp.toString().c_str());
    return result;
  }

  do
  {
    const uint8_t* document;
    uint32_t document_len;
    bson_t temp;
    bson_iter_document(&baz, &document_len, &document);
    if (!bson_init_static(&temp, document, document_len))
      continue;
    result.push_back(BSONDoc(&temp));
  } while (bson_iter_next(&baz));
  return result;
}

int BSONDoc::getType(const std::string& path) const
{
  bson_iter_t iter;
  bson_iter_t baz;
  if (bson_iter_init(&iter, _doc) && bson_iter_find_descendant(&iter, path.c_str(), &baz))
  {
    return bson_iter_type(&baz);
  }
  return 0;
}

bool BSONDoc::removeDuplicates(std::vector<Mongo::BSONDoc>& v, const std::string& key)
{
  if (v.empty())
    return false;

  // https://kodlogs.com/38749/c-remove-duplicates-from-vector-without-sorting
  auto end = v.end();
  for (auto it = v.begin(); it != end; ++it)
  {
    if (!it->has(key))
      continue;

    auto origVal = it->get<std::string>(key);

    end = std::remove_if(it + 1, end, [&origVal, &key](const Mongo::BSONDoc& doc) {
      return doc.get<std::string>(key) == origVal;
    });
  }
  v.erase(end, v.end());

  return true;
}

}  // namespace Mongo
