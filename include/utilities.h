// =====================================================================================
//
//       Filename:  utilities.h
//
//    Description:  various bits of useful code from several projects
//
//        Version:  1.0
//        Created:  01/07/2023 10:01:17 AM
//       Revision:  none
//       Compiler:  g++
//
//         Author:  David P. Riedel (), driedel@cox.net
//        License:  GNU General Public License -v3
//
// =====================================================================================


    /* This file is part of ModernCRecord. */

    /* ModernCRecord is free software: you can redistribute it and/or modify */
    /* it under the terms of the GNU General Public License as published by */
    /* the Free Software Foundation, either version 3 of the License, or */
    /* (at your option) any later version. */

    /* ModernCRecord is distributed in the hope that it will be useful, */
    /* but WITHOUT ANY WARRANTY; without even the implied warranty of */
    /* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
    /* GNU General Public License for more details. */

    /* You should have received a copy of the GNU General Public License */
    /* along with ModernCRecord.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef  _UTILITIES_INC_
#define  _UTILITIES_INC_

#include <filesystem>
#include <format>
#include <string>
#include <string_view>
#include <vector>


// helper type for the visitor #4
template<class... Ts> struct Overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

namespace fs = std::filesystem;

#include <boost/assert.hpp>

//  let's do a little 'template normal' programming again

// function to split a string on a delimiter and return a vector of items.
// use concepts to restrict to strings and string_views.

template<typename T>
inline std::vector<T> SplitString(std::string_view string_data, char delim)
    requires std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>
{
    std::vector<T> results;
	for (size_t it = 0; it < string_data.size(); ++it)
	{
		auto pos = string_data.find(delim, it);
        if (pos != T::npos)
        {
    		results.emplace_back(string_data.substr(it, pos - it));
        }
        else
        {
    		results.emplace_back(string_data.substr(it));
            break;
        }
		it = pos;
	}
    return results;
}

// a (hopefully) efficient way to read an entire file into a string.  Does a binary read. 

std::string LoadDataFileForUse (const fs::path& file_name);

// custom fmtlib formatter for filesytem paths

template <> struct std::formatter<std::filesystem::path>: std::formatter<std::string> 
{
    // parse is inherited from formatter<string>.
    auto format(const std::filesystem::path& p, std::format_context& ctx) const
    {
        std::string f_name;
        std::format_to(std::back_inserter(f_name), "{}", p.string());
        return formatter<std::string>::format(f_name, ctx);
    }
};

#endif   // ----- #ifndef _UTILITIES_INC_  ----- 

