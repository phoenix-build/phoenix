/*
 * (C) 2015-2016 Augustin Cavalier
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once

#include <map>
#include <string>
#include <vector>

#include "build/Generators.h"

class NinjaGenerator : public Generator
{
public:
	NinjaGenerator();
	virtual ~NinjaGenerator();

	virtual bool check() override;

	virtual void setProjectName(const std::string&) override {}

	virtual void setBuildScriptFiles(const std::string& program,
		const std::vector<std::string> files) override;

	virtual void addRegularRule(const std::string& ruleName,
		const std::string& descName, const std::vector<std::string>& forExts,
		const std::string& program, const std::string& outFileExt,
		DependencyFormat depFormat, const std::string& depPrefix,
		const std::string& rule) override;
	virtual void addLinkRule(const std::string& ruleName,
		const std::string& descName, const std::string& program,
		const std::string& rule) override;

	virtual void addTarget(const std::string& linkRule,
	    const std::string& outputBinaryName,
	    const std::vector<std::string>& inputFiles,
	    const std::string& targetFlags, const Target*) override;

	virtual std::vector<std::string> outputFiles() override;
	virtual std::string command(const std::string& target) override;

	virtual void write() override;

private:
	std::string escapeString(const std::string& str);

	std::string fNinjaExecutable;
	bool fFeaturePoolConsole;
	std::string fRequiredVersion;

	typedef struct {
		std::string ruleName;
		std::string outFileExt;
	} RuleForExt;
	std::map<std::string, RuleForExt> fRulesForExts;

	std::string fDepsPrefix;
	std::vector<std::string> fRulesLines;
	std::vector<std::string> fBuildLines;
	std::vector<std::string> fTargets;
};
