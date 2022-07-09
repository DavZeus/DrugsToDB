#include "drug_parser.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <ranges>

auto dr::drug_parser::get_form_by_code(const std::string &code) -> std::string {
  for (const auto &elem : form_codes_tree_) {
    if (elem.second.get<std::string>("code") == code) {
      return elem.second.get<std::string>("name");
    }
  }
  return "";
}

auto dr::drug_parser::find_name(const bpt &drug) -> std::string {
  try {
    std::string result;
    const auto [begin, end] =
        drug.equal_range("hccdo:drugCountryRegistrationDetails");
    for (auto it = begin; it != end; ++it) {
      if (result.empty()) {
        result = it->second.get<std::string>("hcsdo:drugTradeName");
      } else {
        result += '/' + it->second.get<std::string>("hcsdo:drugTradeName");
      }
    }
    return result;
  } catch ([[maybe_unused]] const boost::property_tree::ptree_bad_path &ex) {
  }
  return {};
}

auto dr::drug_parser::find_form(const bpt &drug) -> std::string {
  try {
    std::string res =
        drug.get<std::string>("hccdo:drugDetails.hccdo:packageFormDetails."
                              "hccdo:packageDetails.hcsdo:dosageFormCode");
    res = get_form_by_code(res);
    if (!res.empty()) {
      return res;
    }
  } catch ([[maybe_unused]] const boost::property_tree::ptree_bad_path &ex) {
  }
  try {
    std::string res =
        drug.get<std::string>("hccdo:drugDetails.hccdo:packageFormDetails."
                              "hccdo:packageDetails.hcsdo:dosageFormName");
    return res;
  } catch ([[maybe_unused]] const boost::property_tree::ptree_bad_path &ex) {
  }
  const auto *res = u8"не указано";
  return std::bit_cast<const char *>(res);
}

auto dr::drug_parser::find_owner(const bpt &drug) -> std::string {
  try {
    return drug.get<std::string>(
        "hccdo:registrationCertificateHolderDetails.csdo:businessEntityName");
  } catch ([[maybe_unused]] const boost::property_tree::ptree_bad_path &ex) {
  }
  try {
    return drug.get<std::string>("hccdo:registrationCertificateHolderDetails."
                                 "csdo:businessEntityBriefName");
  } catch ([[maybe_unused]] const boost::property_tree::ptree_bad_path &ex) {
  }
  try {
    return drug.get<std::string>(
        "hccdo:drugDetails.hccdo:manufacturingAuthorizationHolderDetails.csdo:"
        "businessEntityName");
  } catch ([[maybe_unused]] const boost::property_tree::ptree_bad_path &ex) {
  }
  try {
    return drug.get<std::string>(
        "hccdo:drugDetails.hccdo:manufacturingAuthorizationHolderDetails.csdo:"
        "businessEntityBriefName");
  } catch ([[maybe_unused]] const boost::property_tree::ptree_bad_path &ex) {
  }
  const auto *res = u8"не указано";
  return std::bit_cast<const char *>(res);
}

auto dr::drug_parser::find_reg_num(const bpt &drug) -> std::string {
  try {
    return drug.get<std::string>(
        "hccdo:drugCountryRegistrationDetails.hccdo:"
        "drugRegistrationCertificateDetails.hcsdo:registrationCertificateId");
  } catch ([[maybe_unused]] const boost::property_tree::ptree_bad_path &ex) {
  }
  return {};
}

auto dr::drug_parser::find_reg_date(const bpt &drug) -> std::string {
  try {
    auto result =
        drug.get<std::string>("hccdo:drugCountryRegistrationDetails.hccdo:"
                              "drugApplicationDetails."
                              "hcsdo:applicationReceiptDate");
    result.erase(result.begin() + 10, result.end());
    return result;
  } catch ([[maybe_unused]] const boost::property_tree::ptree_bad_path &ex) {
  }
  return {};
}

auto dr::drug_parser::find_term_date(const bpt &drug) -> std::string {
  try {
    auto result =
        drug.get<std::string>("ccdo:resourceItemStatusDetails.ccdo:"
                              "validityPeriodDetails.csdo:startDateTime");
    result.erase(result.begin() + 10, result.end());
    return result;
  } catch ([[maybe_unused]] const boost::property_tree::ptree_bad_path &ex) {
  }
  return {};
}

auto dr::drug_parser::find_validation_date(const bpt &drug) -> std::string {
  try {
    const auto [begin, end] =
        drug.equal_range("hccdo:drugCountryRegistrationDetails");
    for (auto it = begin; it != end; ++it) {
      try {
        std::string result = it->second.get<std::string>(
            "hccdo:drugRegistrationCertificateDetails.csdo:docCreationDate");
        if (!result.empty()) {
          result.erase(result.begin() + 10, result.end());
        }
        return result;
      } catch (
          [[maybe_unused]] const boost::property_tree::ptree_bad_path &ex) {
      }
    }
  } catch ([[maybe_unused]] const boost::property_tree::ptree_bad_path &ex) {
  }
  return {};
}

auto dr::drug_parser::find_status(const bpt &drug) -> std::string {
  // Alternative
  /*try {
    return drug.get<std::string>(
        "hccdo:drugCountryRegistrationDetails.hccdo:"
        "drugRegistrationCertificateDetails.hccdo:registrationStatusName");
  } catch ([[maybe_unused]] const boost::property_tree::ptree_bad_path &ex) {
  }*/
  try {
    auto result = drug.get<std::string>(
        "hccdo:drugCountryRegistrationDetails.hccdo:"
        "drugRegistrationCertificateDetails.hccdo:registrationStatusCode");
    if (result == "99") {
      const auto *other = u8"другое";
      return std::bit_cast<const char *>(other);
    }
  } catch ([[maybe_unused]] const boost::property_tree::ptree_bad_path &ex) {
  }
  const auto *other = u8"зарегистрировано";
  return std::bit_cast<const char *>(other);
}

auto dr::drug_parser::find_country(const bpt &drug) -> std::string {
  try {
    std::string result;
    const auto [begin, end] =
        drug.equal_range("hccdo:drugCountryRegistrationDetails");
    for (auto it = begin; it != end; ++it) {
      if (result.empty()) {
        result = it->second.get<std::string>("csdo:unifiedCountryCode.value");
      } else {
        result +=
            '/' + it->second.get<std::string>("csdo:unifiedCountryCode.value");
      }
    }
    return result;
  } catch ([[maybe_unused]] const boost::property_tree::ptree_bad_path &ex) {
  }
  return {};
}

auto dr::drug_parser::find_update_date(const bpt &drug) -> std::string {
  try {
    auto result = drug.get<std::string>(
        "ccdo:resourceItemStatusDetails.csdo:updateDateTime");
    result.erase(result.begin() + 10, result.end());
    return result;
  } catch ([[maybe_unused]] const boost::property_tree::ptree_bad_path &ex) {
  }
  return {};
}

auto dr::drug_parser::find_id(const bpt &drug) -> std::string {
  try {
    return drug.get<std::string>("hcsdo:_id");
  } catch ([[maybe_unused]] const boost::property_tree::ptree_bad_path &ex) {
  }
  return {};
}

auto dr::drug_parser::process_drug(const bpt &drug)
    -> std::unordered_map<std::string_view, std::string> {
  std::unordered_map<std::string_view, std::string> result;
  result.insert({N_LP, find_name(drug)});
  result.insert({N_FV, find_form(drug)});
  result.insert({COMPANY_DECLARANT, find_owner(drug)});
  result.insert({NREG, find_reg_num(drug)});
  result.insert({DATA, find_reg_date(drug)});
  result.insert({TERM, find_term_date(drug)});
  result.insert({NDATE, find_validation_date(drug)});
  result.insert({STATUS, find_status(drug)});
  result.insert({COUNTRY, find_country(drug)});
  result.insert({UPDATE_DATE, find_update_date(drug)});
  result.insert({ID_STR, find_id(drug)});
  return result;
}

auto dr::drug_parser::remove_attr() -> void {
  drugs_tree_.front().second.erase("<xmlattr>");
}

auto dr::drug_parser::load_codes() -> void {
  boost::property_tree::read_json("codes.json", form_codes_tree_);
}

auto dr::drug_parser::get_drug_data()
    -> std::vector<std::unordered_map<std::string_view, std::string>> {
  std::vector<std::unordered_map<std::string_view, std::string>> result;
  for (const auto &val : drugs_tree_.front().second | std::views::values) {
    result.push_back(process_drug(val));
  }
  return result;
}

auto dr::drug_parser::parse(const std::string &path)
    -> std::vector<std::unordered_map<std::string_view, std::string>> {
  std::vector<std::unordered_map<std::string_view, std::string>> result;
  try {
    boost::property_tree::read_xml(path, drugs_tree_);
    remove_attr();
    load_codes();
    result = get_drug_data();
  } catch (const boost::property_tree::xml_parser_error &ex) {
    fmt::print(stderr, "\n{}\n", ex.what());
  }
  return result;
}
