#ifndef _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#endif

#include "drug_parser.h"

#include <mysqlx/xdevapi.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    const auto *err = u8"Файл не указан";
    fmt::print(stderr, std::bit_cast<const char *>(err));
    return EXIT_FAILURE;
  }
  try {
    dr::drug_parser d_parser;
    std::filesystem::path p = argv[1];
    if (p.extension().empty()) {
      p += ".xml";
    }
    const auto data = d_parser.parse(p.string());
    const std::string user = "root";
    const std::string pwd = "271828182845904523536";
    mysqlx::Session session("localhost", 33060, user, pwd);
    mysqlx::Schema schema = session.getSchema("mydb", true);
    mysqlx::Table table = schema.getTable("drugs", true);
    const auto s = table.count();
    if (s != 0) {
      table.remove().where("true").execute();
    }
    mysqlx::TableInsert insert = table.insert(
        dr::N_LP.data(), dr::N_FV.data(), dr::COMPANY_DECLARANT.data(),
        dr::NREG.data(), dr::DATA.data(), dr::TERM.data(), dr::NDATE.data(),
        dr::STATUS.data(), dr::COUNTRY.data(), dr::UPDATE_DATE.data(),
        dr::ID_STR.data());
    for (const auto &elem : data) {
      insert.values(elem.at(dr::N_LP), elem.at(dr::N_FV),
                    elem.at(dr::COMPANY_DECLARANT), elem.at(dr::NREG),
                    elem.at(dr::DATA), elem.at(dr::TERM), elem.at(dr::NDATE),
                    elem.at(dr::STATUS), elem.at(dr::COUNTRY),
                    elem.at(dr::UPDATE_DATE), elem.at(dr::ID_STR));
    }
    mysqlx::Result r = insert.execute();
    fmt::print("Drugs added to table: {}", r.getAffectedItemsCount());
  } catch (const mysqlx::Error &sql_ex) {
    fmt::print(stderr, "{}\n", sql_ex.what());
    return EXIT_FAILURE;
  } catch (const boost::property_tree::ptree_error &pt_ex) {
    fmt::print(stderr, "{}\n", pt_ex.what());
    return EXIT_FAILURE;
  } catch (const std::exception &std_ex) {
    fmt::print(stderr, "{}\n", std_ex.what());
    return EXIT_FAILURE;
  }
}
