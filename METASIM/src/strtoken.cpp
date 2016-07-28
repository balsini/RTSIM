#include <cstdlib>
#include <strtoken.hpp>

namespace parse_util {

        using namespace std;

        //removes the spaces at the beginning and at the end of the string
        string remove_spaces(const string &tk)
        {
                string temp = tk;
                string::size_type pos = 0;

                if (tk == "") return temp;
                // remove spaces at the beginning...
                temp.erase(0, temp.find_first_not_of(' '));
                pos = temp.find_last_not_of(' ');
                temp.erase(pos+1, temp.size() - pos - 1);
                return temp;
        }

        vector<string> split(const string &code, const string &sep)
        {
                vector<string> temp;
                string::size_type pos = 0;
                string::size_type old_pos = 0;
                int count = 0;

                while (pos != string::npos) {
                        pos = code.find(sep, old_pos);
                        if (pos != string::npos) { 
                                temp.push_back(remove_spaces(code.substr(old_pos,pos-old_pos)));
                                old_pos = pos + sep.size();
                                count ++;
                        }
                        else {
                                temp.push_back(remove_spaces(code.substr(old_pos, code.size()
                                                                         - old_pos)));
                        }
                }

                return temp;
        }

        vector<string> split_instr(const string &code)
        {
                vector<string> temp;
                string::size_type pos = 0;
                string::size_type old_pos = 0;

                while (pos != string::npos) {
                        pos = code.find(';', old_pos);
                        if (pos != string::npos) { 
                                temp.push_back(remove_spaces(code.substr(old_pos,pos-old_pos)));
                                old_pos = ++pos;
                        }
                }

                return temp;
        }

        string get_token(const string &instr, const string &open_par)
        {
                string temp;

                string::size_type pos = instr.find(open_par);
                string::size_type pos1 = instr.find_first_not_of(" \n");

                temp = instr.substr(pos1, pos - pos1);

                return temp;
        }

        string get_param(const string &instr, const string &open_par,
                         const string &close_par)
        {
                string temp("");
                string::size_type pos = instr.find(open_par);

                if (pos != string::npos) {
                        string::size_type end = instr.find_last_of(close_par);
                        temp = instr.substr(pos+1, end-pos-1);
                }
  
                return temp;
        }

        vector<string> split_param(const string &p, const string &sep,
                                   char open_par, char close_par)
        {
                vector<string> temp;
                string::size_type pos = 0;
                string::size_type old_pos = 0;

                string symbols = sep + open_par;

                while (pos <= p.size()) {
                        pos = p.find_first_of(symbols, pos);

                        if (pos != string::npos)
                                if (p[pos] == open_par) {
                                        pos = p.find(close_par, pos);
                                        if (pos == p.size()) pos = string::npos;
                                        else ++pos;
                                }
      
                        if (pos != string::npos) {
                                string t = remove_spaces(p.substr(old_pos, pos - old_pos));

                                temp.push_back(t);

                                old_pos = ++pos;
                        }
                }

                if (pos != old_pos) {
                        string t = remove_spaces(p.substr(old_pos, p.size() - old_pos));

                        if (t != "") temp.push_back(t);
                }

                return temp;
        }


        void parse_double(const string &nums, double &res, string &unit)
        {
                string tmp = remove_spaces(nums);
                string::size_type pos = 0;
                string symb = "smnu";

                pos = tmp.find_first_of(symb, pos);

                if (pos != string::npos) 
                        unit = tmp.substr(pos, tmp.size() - pos);
                else unit = "";  
    
                string snum = tmp.substr(0,pos);
                res = atof(snum.c_str());
        }
        ParseExc::ParseExc(const string &where, const string &par)
                : _where(where), _par(par)
        {
        }

        string ParseExc::what() 
        {
                return "Parse error: in " + _where + " param(s) " + _par + " is unknown"; 
        }
}
