#pragma once

#ifndef FMT_UNICODE
#define FMT_UNICODE 1
#endif

#include <fmt/core.h>

#include <boost/property_tree/ptree.hpp>
#include <filesystem>

namespace dr {
inline static const std::string_view N_LP{"N_LP"};
inline static const std::string_view N_FV{"N_FV"};
inline static const std::string_view COMPANY_DECLARANT{"COMPANY_DECLARANT"};
inline static const std::string_view NREG{"NREG"};
inline static const std::string_view DATA{"DATA"};
inline static const std::string_view TERM{"TERM"};
inline static const std::string_view NDATE{"NDATE"};
inline static const std::string_view STATUS{"STATUS"};
inline static const std::string_view COUNTRY{"COUNTRY"};
inline static const std::string_view UPDATE_DATE{"UPDATE_DATE"};
inline static const std::string_view ID_STR{"ID_STR"};

class drug_parser {
  using bpt = boost::property_tree::ptree;
  [[nodiscard]] auto get_form_by_code(const std::string &code) -> std::string;

  [[nodiscard]] auto find_name(const bpt &drug) -> std::string;
  [[nodiscard]] auto find_form(const bpt &drug) -> std::string;
  [[nodiscard]] auto find_owner(const bpt &drug) -> std::string;
  [[nodiscard]] auto find_reg_num(const bpt &drug) -> std::string;
  [[nodiscard]] auto find_reg_date(const bpt &drug) -> std::string;
  [[nodiscard]] auto find_term_date(const bpt &drug) -> std::string;
  [[nodiscard]] auto find_validation_date(const bpt &drug) -> std::string;
  [[nodiscard]] auto find_status(const bpt &drug) -> std::string;
  [[nodiscard]] auto find_country(const bpt &drug) -> std::string;
  [[nodiscard]] auto find_update_date(const bpt &drug) -> std::string;
  [[nodiscard]] auto find_id(const bpt &drug) -> std::string;

  [[nodiscard]] auto process_drug(const bpt &drug)
      -> std::unordered_map<std::string_view, std::string>;

  auto remove_attr() -> void;
  auto load_codes() -> void;

  bpt form_codes_tree_;

  bpt drugs_tree_;

  [[nodiscard]] auto get_drug_data()
      -> std::vector<std::unordered_map<std::string_view, std::string>>;

public:
  [[nodiscard]] auto parse(const std::string &path)
      -> std::vector<std::unordered_map<std::string_view, std::string>>;
};

} // namespace dr
