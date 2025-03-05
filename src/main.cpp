#include <string>
#include <string_view>

#include "glaze/glaze.hpp"
#include "glaze/glaze_exceptions.hpp"

/*
[[maybe_unused]] constexpr std::string_view json_whitespace = R"(
{
   "fixed_object": {
      "int_array": [0, 1, 2, 3, 4, 5, 6],
      "float_array": [0.1, 0.2, 0.3, 0.4, 0.5, 0.6],
      "double_array": [3288398.238, 233e22, 289e-1, 0.928759872, 0.22222848, 0.1, 0.2, 0.3, 0.4]
   },
   "fixed_name_object": {
      "name0": "James",
      "name1": "Abraham",
      "name2": "Susan",
      "name3": "Frank",
      "name4": "Alicia"
   },
   "another_object": {
      "string": "here is some text",
      "another_string": "Hello World",
      "escaped_text": "{\"some key\":\"some string value\"}",
      "boolean": false,
      "nested_object": {
         "v3s": [[0.12345, 0.23456, 0.001345],
                  [0.3894675, 97.39827, 297.92387],
                  [18.18, 87.289, 2988.298]],
         "id": "298728949872"
      }
   },
   "string_array": ["Cat", "Dog", "Elephant", "Tiger"],
   "string": "Hello world",
   "number": 3.14,
   "boolean": true,
   "another_bool": false
}
)";
*/

//constexpr std::string_view json_minified = R"({"fixed_object":{"int_array":[0,1,2,3,4,5,6],"float_array":[0.1,0.2,0.3,0.4,0.5,0.6],"double_array":[3288398.238,2.33e+24,28.9,0.928759872,0.22222848,0.1,0.2,0.3,0.4]},"fixed_name_object":{"name0":"James","name1":"Abraham","name2":"Susan","name3":"Frank","name4":"Alicia"},"another_object":{"string":"here is some text","another_string":"Hello World","escaped_text":"{\"some key\":\"some string value\"}","boolean":false,"nested_object":{"v3s":[[0.12345,0.23456,0.001345],[0.3894675,97.39827,297.92387],[18.18,87.289,2988.298]],"id":"298728949872"}},"string_array":["Cat","Dog","Elephant","Tiger"],"string":"Hello world","number":3.14,"boolean":true,"another_bool":false})";
constexpr std::string_view json_minified = R"({"map":{{"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"},{"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"},{"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}, {"Name", "asdasdasdasdawdawd"}}})";

#include <chrono>
#include <iostream>
#include <unordered_map>

#include <format>
//#include "boost/describe/class.hpp"

struct fixed_object_t
{
   std::vector<int> int_array;
   std::vector<float> float_array;
   std::vector<double> double_array;
};

struct fixed_name_object_t
{
   std::string name0{};
   std::string name1{};
   std::string name2{};
   std::string name3{};
   std::string name4{};
};

struct nested_object_t
{
   std::vector<std::array<double, 3>> v3s{};
   std::string id{};
};

struct another_object_t
{
   std::string string{};
   std::string another_string{};
   std::string escaped_text{};
   bool boolean{};
   nested_object_t nested_object{};
};

struct obj_t
{
   fixed_object_t fixed_object{};
   fixed_name_object_t fixed_name_object{};
   another_object_t another_object{};
   std::vector<std::string> string_array{};
   std::string string{};
   double number{};
   bool boolean{};
   bool another_bool{};
};

template <>
struct glz::meta<fixed_object_t> {
   using T = fixed_object_t;
   static constexpr auto value = object(
      &T::int_array,
      &T::float_array,
      &T::double_array
   );
};

template <>
struct glz::meta<fixed_name_object_t> {
   using T = fixed_name_object_t;
   static constexpr auto value = object(
      &T::name0,
      &T::name1,
      &T::name2,
      &T::name3,
      &T::name4
   );
};

template <>
struct glz::meta<nested_object_t> {
   using T = nested_object_t;
   static constexpr auto value = object(
      &T::v3s,
      &T::id
   );
};

template <>
struct glz::meta<another_object_t> {
   using T = another_object_t;
   static constexpr auto value = object(
      &T::string,
      &T::another_string,
      &T::escaped_text,
      &T::boolean,
      &T::nested_object
   );
};

template <>
struct glz::meta<obj_t> {
   using T = obj_t;
   static constexpr auto value = object(
      &T::fixed_object,
      &T::fixed_name_object,
      &T::another_object,
      &T::string_array,
      &T::string,
      &T::number,
      &T::boolean,
      &T::another_bool
   );
};

struct qmap_qstring_object
{
    qmap_qstring_object() = default;
    qmap_qstring_object(qmap_qstring_object&) = default;
    qmap_qstring_object(QMap<QString, QString> map) : map(map) {}
    QMap<QString, QString> map;
};

struct qmap_stdvariant_object
{
    qmap_stdvariant_object() = default;
    qmap_stdvariant_object(qmap_stdvariant_object&) = default;
    qmap_stdvariant_object(QMap<QString, std::variant<int, double>> map) : map(map) {}
    QMap<QString, std::variant<int, double>> map;
};


struct qmap_qvariant_object
{
    qmap_qvariant_object() = default;
    qmap_qvariant_object(qmap_qvariant_object&) = default;
    qmap_qvariant_object(QMap<QString, QVariant> map) : map(map) {}
    QMap<QString, QVariant> map;
};

struct qmap_stdstring_object
{
    qmap_stdstring_object() = default;
    qmap_stdstring_object(QMap<std::string, std::string> map) : map(map) {}
    QMap<std::string, std::string> map;
};

struct stdmap_stdstring_object
{
    stdmap_stdstring_object() = default;
    stdmap_stdstring_object(std::map<std::string, std::string> map) : map(map) {}
    std::map<std::string, std::string> map;
};

struct flat_qstring_object
{
    QString name0;
    QString name1;
    QString name2;
    QString name3;
    QString name4;
    QString name5;
    QString name6;
    QString name7;
    QString name8;
    QString name9;
    QString name10;
    QString name11;
    QString name12;
    QString name13;
    QString name14;
    QString name15;
    QString name16;
    QString name17;
    QString name18;
    QString name19;
    QString name20;
    QString name21;
    QString name22;
    QString name23;
    QString name24;
    QString name25;
    QString name26;
    QString name27;
    QString name28;
    QString name29;
    QString name30;
    QString name31;
    QString name32;
    QString name33;
    QString name34;
    QString name35;
    QString name36;
    QString name37;
    QString name38;
    QString name39;
    QString name40;
    QString name41;
    QString name42;
    QString name43;
};

struct flat_stdstring_object
{
    std::string name0;
    std::string name1;
    std::string name2;
    std::string name3;
    std::string name4;
    std::string name5;
    std::string name6;
    std::string name7;
    std::string name8;
    std::string name9;
    std::string name10;
    std::string name11;
    std::string name12;
    std::string name13;
    std::string name14;
    std::string name15;
    std::string name16;
    std::string name17;
    std::string name18;
    std::string name19;
    std::string name20;
    std::string name21;
    std::string name22;
    std::string name23;
    std::string name24;
    std::string name25;
    std::string name26;
    std::string name27;
    std::string name28;
    std::string name29;
    std::string name30;
    std::string name31;
    std::string name32;
    std::string name33;
    std::string name34;
    std::string name35;
    std::string name36;
    std::string name37;
    std::string name38;
    std::string name39;
    std::string name40;
    std::string name41;
    std::string name42;
    std::string name43;
};

template <>
struct glz::meta<qmap_qstring_object> {
    using T = qmap_qstring_object;
    static constexpr auto value = object(
        &T::map
        );
};

template <>
struct glz::meta<qmap_stdvariant_object> {
    using T = qmap_stdvariant_object;
    static constexpr auto value = object(
        &T::map
        );
};


template <>
struct glz::meta<qmap_qvariant_object> {
    using T = qmap_qvariant_object;
    static constexpr auto value = object(
        &T::map
        );
};

template <>
struct glz::meta<qmap_stdstring_object> {
    using T = qmap_stdstring_object;
    static constexpr auto value = object(
        &T::map
        );
};

template <>
struct glz::meta<stdmap_stdstring_object> {
    using T = stdmap_stdstring_object;
    static constexpr auto value = object(
        &T::map
        );
};

template <>
struct glz::meta<flat_qstring_object> {
    using T = flat_qstring_object;
    static constexpr auto value = object(
        &T::name0,
        &T::name1,
        &T::name2,
        &T::name3,
        &T::name4,
        &T::name5,
        &T::name6,
        &T::name7,
        &T::name8,
        &T::name9,
        &T::name10,
        &T::name11,
        &T::name12,
        &T::name13,
        &T::name14,
        &T::name15,
        &T::name16,
        &T::name17,
        &T::name18,
        &T::name19,
        &T::name20,
        &T::name21,
        &T::name22,
        &T::name23,
        &T::name24,
        &T::name25,
        &T::name26,
        &T::name27,
        &T::name28,
        &T::name29,
        &T::name30,
        &T::name31,
        &T::name32,
        &T::name33,
        &T::name34,
        &T::name35,
        &T::name36,
        &T::name37,
        &T::name38,
        &T::name39,
        &T::name40,
        &T::name41,
        &T::name42,
        &T::name43
        );
};

template <>
struct glz::meta<flat_stdstring_object> {
    using T = flat_stdstring_object;
    static constexpr auto value = object(
        &T::name0,
        &T::name1,
        &T::name2,
        &T::name3,
        &T::name4,
        &T::name5,
        &T::name6,
        &T::name7,
        &T::name8,
        &T::name9,
        &T::name10,
        &T::name11,
        &T::name12,
        &T::name13,
        &T::name14,
        &T::name15,
        &T::name16,
        &T::name17,
        &T::name18,
        &T::name19,
        &T::name20,
        &T::name21,
        &T::name22,
        &T::name23,
        &T::name24,
        &T::name25,
        &T::name26,
        &T::name27,
        &T::name28,
        &T::name29,
        &T::name30,
        &T::name31,
        &T::name32,
        &T::name33,
        &T::name34,
        &T::name35,
        &T::name36,
        &T::name37,
        &T::name38,
        &T::name39,
        &T::name40,
        &T::name41,
        &T::name42,
        &T::name43
        );
};

// for testing large, flat documents and out of sequence reading
template <bool backward>
struct abc_t
{
   std::vector<int64_t> a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z;
   bool initialized = init();
   
   bool init() {
      auto fill = [](auto& v) {
         v.resize(1000);
         std::iota(v.begin(), v.end(), 0);
      };
      
      fill(a); fill(b); fill(c);
      fill(d); fill(e); fill(f);
      fill(g); fill(h); fill(i);
      fill(j); fill(k); fill(l);
      fill(m); fill(n); fill(o);
      fill(p); fill(q); fill(r);
      fill(s); fill(t); fill(u);
      fill(v); fill(w); fill(x);
      fill(y); fill(z);
      return true;
   }
};

template <>
struct glz::meta<abc_t<false>>
{
   using T = abc_t<false>;
   static constexpr auto value = object(&T::a,&T::b,&T::c,&T::d,&T::e,&T::f,&T::g,&T::h,&T::i,&T::j,&T::k,&T::l,&T::m,&T::n,
                                        &T::o,&T::p,&T::q,&T::r,&T::s,&T::t,&T::u,&T::v,&T::w,&T::x,&T::y,&T::z);
};

template <>
struct glz::meta<abc_t<true>>
{
   using T = abc_t<true>;
   static constexpr auto value = object(&T::z,&T::y,&T::x,&T::w,&T::v,&T::u,&T::t,&T::s,&T::r,&T::q,&T::p,&T::o,&T::n,
                                        &T::m,&T::l,&T::k,&T::j,&T::i,&T::h,&T::g,&T::f,&T::e,&T::d,&T::c,&T::b,&T::a);
};

#ifdef NDEBUG
static constexpr size_t iterations = 1'000'000;
static constexpr size_t iterations_abc = 10'000;
#else
static constexpr size_t iterations = 100'000;
static constexpr size_t iterations_abc = 1'000;
#endif

// We scale all speeds by the minified JSON byte length, so that libraries which do not efficiently write JSON do not get an unfair advantage
// We want to know how fast the libraries will serialize/deserialize with repsect to one another
size_t minified_byte_length{};

struct results
{
   std::string_view name{};
   std::string_view url{};
   size_t iterations{};
   
   std::optional<size_t> json_byte_length{};
   std::optional<double> json_read{};
   std::optional<double> json_write{};
   std::optional<double> json_roundtrip{};
   
   std::optional<size_t> binary_byte_length{};
   std::optional<double> binary_write{};
   std::optional<double> binary_read{};
   std::optional<double> binary_roundtrip{};
   
   void print(bool use_minified = true)
   {
      if (json_roundtrip) {
         std::cout << name << " json roundtrip: " << *json_roundtrip << " s\n";
      }
      
      if (json_byte_length) {
         std::cout << name << " json byte length: " << *json_byte_length << '\n';
      }
      
      if (json_write) {
         if (json_byte_length) {
            const auto byte_length = use_minified ? minified_byte_length : *json_byte_length;
            const auto MBs = iterations * byte_length / (*json_write * 1048576);
            std::cout << name << " json write: " << *json_write << " s, " << MBs << " MB/s\n";
         }
         else {
            std::cout << name << " json write: " << *json_write << " s\n";
         }
      }
      
      if (json_read) {
         if (json_byte_length) {
            const auto byte_length = use_minified ? minified_byte_length : *json_byte_length;
            const auto MBs = iterations * byte_length / (*json_read * 1048576);
            std::cout << name << " json read: " << *json_read << " s, " << MBs << " MB/s\n";
         }
         else {
            std::cout << name << " json read: " << *json_read << " s\n";
         }
      }
      
      if (binary_roundtrip) {
         std::cout << '\n';
         std::cout << name << " binary roundtrip: " << *binary_roundtrip << " s\n";
      }
      
      if (binary_byte_length) {
         std::cout << name << " binary byte length: " << *binary_byte_length << '\n';
      }
      
      if (binary_write) {
         if (binary_byte_length) {
            const auto MBs = iterations * *binary_byte_length / (*binary_write * 1048576);
            std::cout << name << " binary write: " << *binary_write << " s, " << MBs << " MB/s\n";
         }
         else {
            std::cout << name << " binary write: " << *binary_write << " s\n";
         }
      }
      
      if (binary_read) {
         if (binary_byte_length) {
            const auto MBs = iterations * *binary_byte_length / (*binary_read * 1048576);
            std::cout << name << " binary read: " << *binary_read << " s, " << MBs << " MB/s\n";
         }
         else {
            std::cout << name << " binary read: " << *binary_read << " s\n";
         }
      }
      
      std::cout << "\n---\n" << std::endl;
   }
   
   std::string json_stats(bool use_minified = true) const {
      static constexpr std::string_view s = R"(| [**{}**]({}) | **{}** | **{}** | **{}** |)";
      const std::string roundtrip = json_roundtrip ? std::format("{:.2f}", *json_roundtrip) : "N/A";
      if (json_byte_length) {
         const auto byte_length = use_minified ? minified_byte_length : *json_byte_length;
         const std::string write = json_write ? std::format("{}", static_cast<size_t>(iterations * byte_length / (*json_write * 1048576))) : "N/A";
         const std::string read = json_read ? std::format("{}", static_cast<size_t>(iterations * byte_length / (*json_read * 1048576)))  : "N/A";
         return std::format(s, name, url, roundtrip, write, read);
      }
      else {
         const std::string write = json_write ? std::format("{:.2f}", *json_write)  : "N/A";
         const std::string read = json_read ? std::format("{:.2f}", *json_read)  : "N/A";
         return std::format(s, name, url, roundtrip, write, read);
      }
   }
   
   std::string json_stats_read(bool use_minified = true) const {
      static constexpr std::string_view s = R"(| [**{}**]({}) | **{}** |)";
      if (json_byte_length) {
         const auto byte_length = use_minified ? minified_byte_length : *json_byte_length;
         const std::string read = json_read ? std::format("{}", static_cast<size_t>(iterations * byte_length / (*json_read * 1048576)))  : "N/A";
         return std::format(s, name, url, read);
      }
      else {
         const std::string read = json_read ? std::format("{:.2f}", *json_read)  : "N/A";
         return std::format(s, name, url, read);
      }
   }
};

template <class T>
inline bool is_valid_write(const std::string& buffer, const std::string& library_name) {
   T obj{};
   
   glz::ex::read_json(obj, json_minified);
   
   std::string reference = glz::write_json(obj).value();
   
   obj = {};
   glz::ex::read_json(obj, buffer);
   
   std::string compare = glz::write_json(obj).value();
   
   if (reference != compare) {
      std::cout << "Invalid write for library: " << library_name << std::endl;
      return false;
   }
   
   return true;
}

template <glz::opts Opts, typename T>
auto glaze_test(const std::string& testName)
{
   std::string buffer{};

   T obj{{{"Name", "asdasdasdasdawdawd"}, {"Name1", "asdasdasdasdawdawd"}, {"Name2", "asdasdasdasdawdawd"}, {"Name3", "asdasdasdasdawdawd"}, {"Name4", "asdasdasdasdawdawd"}, {"Name5", "asdasdasdasdawdawd"}, {"Name6", "asdasdasdasdawdawd"}, {"Name7", "asdasdasdasdawdawd"}, {"Name8", "asdasdasdasdawdawd"}, {"Name9", "asdasdasdasdawdawd"}, {"Name10", "asdasdasdasdawdawd"}, {"Name11", "asdasdasdasdawdawd"},{"Name12", "asdasdasdasdawdawd"}, {"Name13", "asdasdasdasdawdawd"}, {"Name14", "asdasdasdasdawdawd"}, {"Name15", "asdasdasdasdawdawd"}, {"Name16", "asdasdasdasdawdawd"}, {"Name17", "asdasdasdasdawdawd"}, {"Name18", "asdasdasdasdawdawd"}, {"Name19", "asdasdasdasdawdawd"}, {"Name20", "asdasdasdasdawdawd"}, {"Name21", "asdasdasdasdawdawd"}, {"Name22", "asdasdasdasdawdawd"}, {"Name23", "asdasdasdasdawdawd"}, {"Name24", "asdasdasdasdawdawd"}, {"Name25", "asdasdasdasdawdawd"}, {"Name26", "asdasdasdasdawdawd"}, {"Name27", "asdasdasdasdawdawd"}, {"Name28", "asdasdasdasdawdawd"}, {"Name29", "asdasdasdasdawdawd"}, {"Name30", "asdasdasdasdawdawd"}, {"Name31", "asdasdasdasdawdawd"}, {"Name32", "asdasdasdasdawdawd"}, {"Name33", "asdasdasdasdawdawd"}, {"Name34", "asdasdasdasdawdawd"},{"Name35", "asdasdasdasdawdawd"}, {"Name36", "asdasdasdasdawdawd"}, {"Name37", "asdasdasdasdawdawd"}, {"Name38", "asdasdasdasdawdawd"}, {"Name39", "asdasdasdasdawdawd"}, {"Name40", "asdasdasdasdawdawd"}, {"Name41", "asdasdasdasdawdawd"}, {"Name42", "asdasdasdasdawdawd"}, {"Name43", "asdasdasdasdawdawd"}, {"Name44", "asdasdasdasdawdawd"}, {"Name45", "asdasdasdasdawdawd"}}};

   auto t0 = std::chrono::steady_clock::now();
    /*
   for (size_t i = 0; i < iterations; ++i) {
       auto error = glz::read<Opts>(obj, buffer);
      if (error) {
         std::cout << "glaze error! " << error.custom_error_message << "\n";
         break;
      }
      auto error2 = glz::write_json(obj, buffer);
      if (error2) {
         std::cout << "glaze error! " << error2.custom_error_message << "\n";
         break;
      }
   }

   r.json_roundtrip = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
*/

   auto t1 = std::chrono::steady_clock::now();
   // write performance
   results r{testName, "https://github.com/stephenberry/glaze", iterations };
   t0 = std::chrono::steady_clock::now();
   
   for (size_t i = 0; i < iterations; ++i) {
       auto error = glz::write<Opts>(obj, buffer);
      if (error) {
         std::cout << "glaze error!" << error.custom_error_message << " \n";
         break;
      }
   }
   
   t1 = std::chrono::steady_clock::now();
   
   r.json_byte_length = buffer.size();
   minified_byte_length = *r.json_byte_length;
   r.json_write = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
   
   //is_valid_write<T>(buffer, "Glaze");
   
   // read performance
   
   t0 = std::chrono::steady_clock::now();
   
   for (size_t i = 0; i < iterations; ++i) {
       auto error = glz::read_json(obj, buffer);
      if (error) {
         std::cout << "glaze error!" << error.custom_error_message << " \n";
         break;
      }
   }
   
   t1 = std::chrono::steady_clock::now();
   
   r.json_read = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
   
   // binary write performance

   /*
   t0 = std::chrono::steady_clock::now();
   for (size_t i = 0; i < iterations; ++i) {
      if (glz::write_beve(obj, buffer)) {
         std::cout << "glaze error!\n";
         break;
      }
   }
   
   t1 = std::chrono::steady_clock::now();
   
   r.binary_byte_length = buffer.size();
   r.binary_write = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
   
   // binary read performance
   
   t0 = std::chrono::steady_clock::now();
   
   for (size_t i = 0; i < iterations; ++i) {
      if (glz::read_beve(obj, buffer)) {
         std::cout << "glaze error!\n";
         break;
      }
   }
   
   t1 = std::chrono::steady_clock::now();
   
   r.binary_read = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
   
   // binary round trip
   
   t0 = std::chrono::steady_clock::now();
   
   for (size_t i = 0; i < iterations; ++i) {
      if (glz::read_beve(obj, buffer)) {
         std::cout << "glaze error!\n";
         break;
      }
      if (glz::write_beve(obj, buffer)) {
         std::cout << "glaze error!\n";
         break;
      }
   }
   
   t1 = std::chrono::steady_clock::now();
   
   r.binary_roundtrip = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
*/
   //std::cout << buffer << "\n";
   
   r.print();
   
   return r;
}

template <glz::opts Opts, typename T>
auto glaze_test_qmap_variant(const std::string& testName)
{
    std::string buffer{ json_minified };

    T obj{{{"Name", int(12345678912345)}, {"Name1", int(12345678912345)}, {"Name2", int(12345678912345)}, {"Name3", int(12345678912345)}, {"Name4", int(12345678912345)}, {"Name5", int(12345678912345)}, {"Name6", int(12345678912345)}, {"Name7", int(12345678912345)}, {"Name8", int(12345678912345)}, {"Name9", int(12345678912345)}, {"Name10", int(12345678912345)}, {"Name11", int(12345678912345)},{"Name12", int(12345678912345)}, {"Name13", int(12345678912345)}, {"Name14", int(12345678912345)}, {"Name15", int(12345678912345)}, {"Name16", int(12345678912345)}, {"Name17", int(12345678912345)}, {"Name18", int(12345678912345)}, {"Name19", int(12345678912345)}, {"Name20", int(12345678912345)}, {"Name21", int(12345678912345)}, {"Name22", int(12345678912345)}, {"Name23", int(12345678912345)}, {"Name24", int(12345678912345)}, {"Name25", int(12345678912345)}, {"Name26", int(12345678912345)}, {"Name27", int(12345678912345)}, {"Name28", int(12345678912345)}, {"Name29", int(12345678912345)}, {"Name30", int(12345678912345)}, {"Name31", int(12345678912345)}, {"Name32", int(12345678912345)}, {"Name33", int(12345678912345)}, {"Name34", int(12345678912345)},{"Name35", int(12345678912345)}, {"Name36", int(12345678912345)}, {"Name37", int(12345678912345)}, {"Name38", int(12345678912345)}, {"Name39", int(12345678912345)}, {"Name40", int(12345678912345)}, {"Name41", int(12345678912345)}, {"Name42", int(12345678912345)}, {"Name43", int(12345678912345)}, {"Name44", int(12345678912345)}, {"Name45", int(12345678912345)}}};

    auto t0 = std::chrono::steady_clock::now();

    auto t1 = std::chrono::steady_clock::now();
    // write performance
    results r{testName, "https://github.com/stephenberry/glaze", iterations };
    t0 = std::chrono::steady_clock::now();

    for (size_t i = 0; i < iterations; ++i) {
        auto error = glz::write<Opts>(obj, buffer);
        if (error) {
            std::cout << "glaze error!" << error.custom_error_message << " \n";
            break;
        }
    }

    t1 = std::chrono::steady_clock::now();

    r.json_byte_length = buffer.size();
    minified_byte_length = *r.json_byte_length;
    r.json_write = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;

    //is_valid_write<T>(buffer, "Glaze");

    // read performance

    t0 = std::chrono::steady_clock::now();

    for (size_t i = 0; i < iterations; ++i) {
        auto error = glz::read_json(obj, buffer);
        if (error) {
            std::cout << "glaze error!" << error.custom_error_message << " \n";
            break;
        }
    }

    t1 = std::chrono::steady_clock::now();

    r.json_read = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;

    std::cout << buffer << "\n";

    r.print();

    return r;
}

template <glz::opts Opts, class T>
auto glaze_test_flat_string(const std::string& testName)
{
    std::string buffer{ json_minified };

    T obj{"asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd", "asdasdasdasdawdawd"};

    auto t0 = std::chrono::steady_clock::now();

    auto t1 = std::chrono::steady_clock::now();
    // write performance
    results r{testName, "https://github.com/stephenberry/glaze", iterations };
    t0 = std::chrono::steady_clock::now();

    for (size_t i = 0; i < iterations; ++i) {
        auto error = glz::write<Opts>(obj, buffer);
        if (error) {
            std::cout << "glaze error!" << error.custom_error_message << " \n";
            break;
        }
    }

    t1 = std::chrono::steady_clock::now();

    r.json_byte_length = buffer.size();
    minified_byte_length = *r.json_byte_length;
    r.json_write = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;

    //is_valid_write<T>(buffer, "Glaze");

    // read performance

    t0 = std::chrono::steady_clock::now();

    for (size_t i = 0; i < iterations; ++i) {
        auto error = glz::read_json(obj, buffer);
        if (error) {
            std::cout << "glaze error!" << error.custom_error_message << " \n";
            break;
        }
    }

    t1 = std::chrono::steady_clock::now();

    r.json_read = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;

    //std::cout << buffer << "\n";

    r.print();

    return r;
}

static constexpr std::string_view table_header = R"(
| Library                                                      | Roundtrip Time (s) | Write (MB/s) | Read (MB/s) |
| ------------------------------------------------------------ | ------------------ | ------------ | ----------- |)";

static constexpr std::string_view table_header_read = R"(
| Library                                                      | Read (MB/s) |
| ------------------------------------------------------------ | ----------- |)";

void test0()
{
   std::vector<results> results;
   //results.emplace_back(glaze_test<glz::opts{}, qmap_qstring_object>("QMap<QString, QString>"));
   results.emplace_back(glaze_test_qmap_variant<glz::opts{}, qmap_qvariant_object>("QMap<QString, QVariant>"));
   results.emplace_back(glaze_test_qmap_variant<glz::opts{}, qmap_stdvariant_object>("QMap<QString, stdvariant>"));
   //results.emplace_back(glaze_test<glz::opts{}, qmap_stdstring_object>("QMap<std::string, std::string>"));
   //results.emplace_back(glaze_test<glz::opts{}, stdmap_stdstring_object>("std::map<std::string, std::string>"));
   //results.emplace_back(glaze_test_flat_string<glz::opts{}, flat_qstring_object>("flat_qstring_object"));
   //results.emplace_back(glaze_test_flat_string<glz::opts{}, flat_stdstring_object>("flat_stdstring_object"));
   
   std::ofstream table{ "json_minfied_stats.md" };
   if (table) {
      const auto n = results.size();
      table << table_header << '\n';
      for (size_t i = 0; i < n; ++i) {
         table << results[i].json_stats();
         if (i != n - 1) {
            table << '\n';
         }
      }
   }
}

int main()
{
   test0();
   return 0;
}
