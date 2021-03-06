#include <metaset.hpp>

#include <iostream>
#include <string>

#include <kdb.hpp>
#include <cmdline.hpp>

using namespace std;
using namespace kdb;

MetaSetCommand::MetaSetCommand()
{}

int MetaSetCommand::execute (Cmdline const& cl)
{
	if (cl.arguments.size() != 3)
	{
		throw invalid_argument ("Need 3 arguments");
	}
	string keyname = cl.arguments[0];
	string metaname = cl.arguments[1];

	KeySet conf;
	Key parentKey(keyname, KEY_END);
	kdb.get(conf, parentKey);
	Key k = conf.lookup(keyname);

	if (!k)
	{
		k = Key(keyname, KEY_END);
		conf.append(k);
	}
	if (!k.isValid())
	{
		cout << "Could not create key" << endl;
		return 1;
	}

	std::string metavalue = cl.arguments[2];
	if (metaname == "atime" || metaname == "mtime" || metaname == "ctime")
	{
		stringstream str (metavalue);
		time_t t;
		str >> t;
		if (!str.good()) throw "conversion failure";
		k.setMeta<time_t> (metaname, t);
	} else {
		k.setMeta<string> (metaname, metavalue);
	}

	kdb.set(conf,parentKey);
	printWarnings(cerr,parentKey);

	return 0;
}

MetaSetCommand::~MetaSetCommand()
{}
