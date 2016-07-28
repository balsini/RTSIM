#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <entity.hpp>
#include "myentity.hpp"

#include "catch.hpp"

using namespace MetaSim;
using namespace std;

TEST_CASE("ParseUtil", "split")
{
    string a = "token1 token2 token3";
    string b = "instr1; instr2; instr3;";
    string c = "code1(a,b); code2(a,b); code3(a, b, c);";
    string d = "set 123.34 234.45 345.56";
  
    vector<string> tokens = parse_util::split(a, " ");
 
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == "token1");
    REQUIRE(tokens[1] == "token2");
    REQUIRE(tokens[2] == "token3");

    tokens = parse_util::split_instr(b);
  
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == "instr1");
    REQUIRE(tokens[1] == "instr2");
    REQUIRE(tokens[2] == "instr3");

    tokens = parse_util::split_instr(c);
  
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == "code1(a,b)");
    REQUIRE(tokens[1] == "code2(a,b)");
    REQUIRE(tokens[2] == "code3(a, b, c)");
}

TEST_CASE("ParseUtil2", "getparam")
{
    string a = "token1 token2 token3";
    string b = "instr1; instr2; instr3;";
    string c = "code1(a,b); code2(a,b); code3(a, b, c);";
    string d = "set 123.34 234.45 345.56";

    vector<string> tokens;

    tokens = parse_util::split_instr(c);
    REQUIRE(tokens.size() == 3);

    string tok = parse_util::get_token(tokens[0]);
    string param = parse_util::get_param(tokens[0]);
    REQUIRE(tok == "code1");
    REQUIRE(param == "a,b");

    tok = parse_util::get_token(tokens[1]);
    param = parse_util::get_param(tokens[1]);
    REQUIRE(tok == "code2");
    REQUIRE(param == "a,b");

    tok = parse_util::get_token(tokens[2]);
    param = parse_util::get_param(tokens[2]);
    REQUIRE(tok == "code3");
    REQUIRE(param == "a, b, c");
}

TEST_CASE("ParseUtil3", "splitparam")
{
    string a = "token1 token2 token3";
    string b = "instr1; instr2; instr3;";
    string c = "code1(a,b); code2(a,b); code3(a, b, c);";
    string d = "set 123.34 234.45 345.56";


    // your code here
    vector<string> tokens;
    string paramlist;
    vector<string> params;

    tokens = parse_util::split_instr(c);
    REQUIRE(tokens.size() == 3);

    paramlist = parse_util::get_param(tokens[0]);

    params = parse_util::split_param(paramlist);
    REQUIRE(params.size() == 2);
    REQUIRE(params[0] == "a");
    REQUIRE(params[1] == "b");

    paramlist = parse_util::get_param(tokens[1]);

    params = parse_util::split_param(paramlist);
    REQUIRE(params.size() == 2);
    REQUIRE(params[0] == "a");
    REQUIRE(params[1] == "b");

    paramlist = parse_util::get_param(tokens[2]);

    params = parse_util::split_param(paramlist);
    REQUIRE(params.size() == 3);
    REQUIRE(params[0] == "a");
    REQUIRE(params[1] == "b");
    REQUIRE(params[2] == "c");
}

TEST_CASE("ParseUtils4", "parsedouble")
{
    string a = "token1 token2 token3";
    string b = "instr1; instr2; instr3;";
    string c = "code1(a,b); code2(a,b); code3(a, b, c);";
    string d = "set 123.34 234.45 345.56";

    // your code here
    vector<string> tokens;
    string unit = "";
    double n;

    tokens = parse_util::split(d, " ");

    REQUIRE(tokens.size() == 4);
    REQUIRE(tokens[0] == "set");
    REQUIRE(tokens[1] == "123.34");
    REQUIRE(tokens[2] == "234.45");
    REQUIRE(tokens[3] == "345.56");
  
    parse_util::parse_double(tokens[0], n, unit);
    REQUIRE(n == 0);

    parse_util::parse_double(tokens[1], n, unit);
    REQUIRE(n == 123.34);

    parse_util::parse_double("3s", n, unit);
    REQUIRE(n == 3);
    REQUIRE(unit == "s");
    parse_util::parse_double(".3s", n, unit);
    REQUIRE(n == 0.3);
    REQUIRE(unit == "s");
    parse_util::parse_double(".3us", n, unit);
    REQUIRE(n == 0.3);
    REQUIRE(unit == "us");
}

