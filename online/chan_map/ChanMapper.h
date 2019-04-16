#ifndef __CHAN_MAPPER_H__
#define __CHAN_MAPPER_H__
#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "ChanMapperRange.h"
class DbSvc;

class ChanMapper {
 protected:
  std::string m_dir_base;
  std::string m_label;
  std::string m_map_id;
  std::string m_header;
  ChanMapperRange m_range;

 public:
  ChanMapper();
  virtual ~ChanMapper();

  std::string GetMapID() { return m_map_id; }
  void        SetMapID(const std::string map_id) { m_map_id = map_id; }
  void SetMapIDbyFile(const int run);
  void SetMapIDbyDB  (const int run);

  std::string RangeFileName();
  std::string MapFileName();
  std::string SchemaName();
  std::string MapTableName();

  void ReadFromFile();
  void  WriteToFile();
  void ReadFromLocalFile(const std::string fn_tsv);
  void  WriteToLocalFile(const std::string fn_tsv);

  void ReadFromDB();
  void WriteToDB ();
  void WriteRangeToDB();

  virtual void Print(std::ostream& os);

 protected:
  typedef std::vector<std::string> LineList;
  virtual int  ReadFileCont(LineList& lines) {;}
  virtual int WriteFileCont(std::ostream& os) {;}

  virtual void  ReadDbTable(DbSvc& db);
  virtual void WriteDbTable(DbSvc& db);
};

#endif // __CHAN_MAPPER_H__
