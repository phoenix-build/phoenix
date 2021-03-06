/*
 * (C) 2015-2016 Augustin Cavalier
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <cstdlib>
#include <algorithm>

#include "Tester.h"

#include "util/StringUtil.h"
#include "util/FSUtil.h"
#include "util/XmlUtil.h"

int main(int, char* argv[])
{
	Tester t(true);

	t.beginGroup("StringUtil");
	t.result(StringUtil::count("0 1 2 3 4", " ") == 4, "count-1");
	t.result(StringUtil::count("Not many 'a's in this sentence.", "a") == 2, "count-2");
	t.result(StringUtil::count("creatively correcting complex concatenations", " co") == 3, "count-3");

	std::vector<std::string> split1 = StringUtil::split("-C:C++:/usr/bin/who/cares/really", ":");
	t.result(split1.size() == 3, "split-1.size");
	t.result(split1[0] == "-C", "split-1[0]");
	t.result(split1[2] == "/usr/bin/who/cares/really", "split-1[2]");
	std::vector<std::string> split2 = StringUtil::split("longer_-?things?-_to()split&&by\n_-?but", "_-?");
	t.result(split2.size() == 3, "split-2.size");
	t.result(split2[0] == "longer", "split-2[0]");
	t.result(split2[2] == "but", "split-2[2]");

	t.result(StringUtil::join({"this is", "", "sparta"}, " not ", false) ==
		"this is not  not sparta", "join-1#skipEmpty=false");
	t.result(StringUtil::join({"this is", "", "sparta"}, " not ", true) ==
		"this is not sparta", "join-1#skipEmpty=true");
	t.result(StringUtil::join({"Hello", "world", "it", "is", "I"}, " ") ==
		"Hello world it is I", "join-2");

	t.result(StringUtil::trim("original string") == "original string", "trim-1");
	t.result(StringUtil::trim("   lots   \tof     whitespace\t  ") ==
		"lots   \tof     whitespace", "trim-2");

	t.result(StringUtil::startsWith("Hay stack", "Hay"), "startsWith-1");
	t.result(StringUtil::startsWith("Haystack", "Hay"), "startsWith-2");
	t.result(!StringUtil::startsWith("Heystack", "Hay"), "startsWith-3");
	t.result(!StringUtil::startsWith("\nHaystack", "Hay"), "startsWith-4");

	t.result(StringUtil::endsWith("\nHaystack", "ack"), "endsWith-1");
	t.result(StringUtil::endsWith("\nHayst ack", "ack"), "endsWith-2");
	t.result(!StringUtil::endsWith("\nHaysteck", "ack"), "endsWith-3");
	t.result(!StringUtil::endsWith("\nHayst eck", "ack"), "endsWith-4");

	std::string replace = "stRing sub_ject StRING tO re_place-_IN_SIDE";
	StringUtil::replaceAll(replace, "_", "");
	t.result(replace ==	"stRing subject StRING tO replace-INSIDE", "replaceAll-1");

	replace = "stRing subject StRING tO replace-INSIDE";
	StringUtil::replaceAll(replace, "RING", "ring");
	t.result(replace ==	"stRing subject String tO replace-INSIDE", "replaceAll-2");
	t.endGroup();

	t.beginGroup("FSUtil");
	t.result(FSUtil::exists(argv[0]), "exists-1");
	t.result(!FSUtil::exists("this_file_does_not_exist.txt"), "exists-2");

	t.result(FSUtil::isFile(argv[0]), "isFile-1");
	t.result(!FSUtil::isFile("this_file_does_not_exist.txt"), "isFile-2");
	t.result(!FSUtil::isFile(".."), "isFile-3");

	t.result(FSUtil::isDir(".."), "isDir-1");
	t.result(FSUtil::isDir("/"), "isDir-2");
	t.result(!FSUtil::isDir(argv[0]), "isDir-3");
	t.result(!FSUtil::isDir("this_file_does_not_exist.txt"), "isDir-4");

	t.result(FSUtil::isExec(argv[0]), "isExec-1");
	t.result(!FSUtil::isExec("this_file_does_not_exist.txt"), "isExec-2");
	t.result(!FSUtil::isExec("."), "isExec-3");

	FSUtil::setContents("this_file_exists.txt", "These are the contents of this file.");
	t.result(FSUtil::isFile("this_file_exists.txt"), "putContents-1/isFile-4");
	t.result(FSUtil::exists("this_file_exists.txt"), "putContents-2");

	t.result(FSUtil::isPathAbsolute("/Nc/this/is/not/path.txt"), "isPathAbsolute-1");
	t.result(!FSUtil::isPathAbsolute("not/absolute/path"), "isPathAbsolute-2");
#ifdef _WIN32
	t.result(FSUtil::isPathAbsolute("C:/what/ever/this/is/anyway/"), "isPathAbsolute-win_1");
#else
	t.result(!FSUtil::isPathAbsolute("C:/what/ever/this/is/anyway/"), "isPathAbsolute-!win_1");
#endif

	t.result(FSUtil::getContents("this_file_exists.txt") == "These are the contents of this file.",
		"getContents-1");
	t.result(FSUtil::getContents("this_file_does_not_exist.txt") == "", "getContents-2");
	FSUtil::setContents("this_file_exists.txt", "These are the modified contents of this file.");
	t.result(FSUtil::getContents("this_file_exists.txt") == "These are the modified contents of this file.",
		"getContents-3/putContents-3");

	FSUtil::deleteFile("this_file_exists.txt");
	t.result(!FSUtil::exists("this_file_exists.txt"), "deleteFile-1/exists-3");

	std::string dir = FSUtil::parentDirectory(__FILE__);
	const std::vector<std::string> files = FSUtil::searchForFiles(dir, {".cpp"}, false);
	bool pass = files.size() == 3 &&
		std::find(files.begin(), files.end(), FSUtil::combinePaths({dir, "ScriptTest.cpp"})) != files.end() &&
		std::find(files.begin(), files.end(), FSUtil::combinePaths({dir, "Tester.cpp"})) != files.end() &&
		std::find(files.begin(), files.end(), FSUtil::combinePaths({dir, "UtilTest.cpp"})) != files.end();
	t.result(pass, "searchForFiles-1");

	const std::vector<std::string> files2 = FSUtil::searchForFiles(dir, {".phnx"}, false);
	t.result(files2.size() == 0, "searchForFiles-2");

	const std::vector<std::string> files3 =
		FSUtil::searchForFiles(FSUtil::combinePaths({dir, "../src/"}), {"Util.h"}, true);
	t.result(files3.size() == 6, "searchForFiles-3");

	// We can't really do much here besides test that it actually finds something.
	t.result(!FSUtil::which("find").empty(), "which-1");

	t.result(FSUtil::normalizePath("/whatever/this//is//../././../whatever/dir2/dir9/apathto.txt") ==
		"/whatever/whatever/dir2/dir9/apathto.txt", "normalizePath-1");
#ifdef _WIN32
	t.result(FSUtil::normalizePath("C:\\directory\\otherdirectory/someplace\\..\\../.\\wow") ==
		"C:/directory/wow", "normalizePath-win_1");
	t.result(FSUtil::normalizePath("/c/this/not/really/../../is/a/MSYS/or/Cygwin/path.txt") ==
		"C:/this/is/a/MSYS/or/Cygwin/path.txt", "normalizePath-win_2");
	t.result(FSUtil::normalizePath("/Nc/this/is/not/././is/../a/MSYS/or/Cygwin/path.txt") ==
		"/Nc/this/is/not/a/MSYS/or/Cygwin/path.txt", "normalizePath-win_3");
#else
	t.result(FSUtil::normalizePath("/c/this/not/really/../../is/a/MSYS/or/Cygwin/path.txt") ==
		"/c/this/is/a/MSYS/or/Cygwin/path.txt", "normalizePath-!win_1");
#endif

	t.result(FSUtil::combinePaths({"/whatever/", "this//is//", ".././",
			"/./../", "whatever/dir2/", "/dir9/apathto.txt"}) ==
		"/whatever/whatever/dir2/dir9/apathto.txt", "combinePaths-1");
	t.result(FSUtil::combinePaths({"/this/not", "really", "", "../..", "is/a//path.txt"}) ==
		"/this/is/a/path.txt", "combinePaths-2");
	t.result(FSUtil::combinePaths({"shortpath", "../../data/languages/"}) ==
		"../data/languages", "combinePaths-3");

	t.result(FSUtil::parentDirectory("/test/1.3.4.2/blah/") == "/test/1.3.4.2",
		"parentDirectory-1");
	t.result(FSUtil::parentDirectory("/test/1.3.4.2/blah/file.txt") == "/test/1.3.4.2/blah",
		"parentDirectory-2");
	t.result(FSUtil::parentDirectory("file.txt") == ".",
		"parentDirectory-3");

	FSUtil::mkdir("this_directory_now_exists");
	t.result(FSUtil::isDir("this_directory_now_exists"), "mkdir-1/isDir-5");
	FSUtil::rmdir("this_directory_now_exists");
	t.result(!FSUtil::isDir("this_directory_now_exists"), "rmdir-1/isDir-6");
	t.endGroup();

	t.beginGroup("XmlUtil");
	XmlGenerator gen("test_tag", {{"bla", "tru"}, {"wat","els"}});
	gen.beginTag("hello_world");
	gen.beginTag("thing", {{"ok", "nop!e\"<>&"}}, true);
	t.result(gen.finish() == "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<test_tag bla=\"tru\" wat=\"els\">\n\t<hello_world>\n\t\t<thing "
		"ok=\"nop!e&quot;&lt;&gt;&amp;\"/>\n\t</hello_world>\n</test_tag>\n", "xmlgen-1");
	t.endGroup();

	return t.done();
}
