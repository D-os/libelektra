#ifndef ELEKTRA_KDB_IO_HPP
#define ELEKTRA_KDB_IO_HPP

#ifndef USER_DEFINED_IO

#include <key.hpp>

#include <ostream>
#include <iomanip>

inline std::ostream & printError(std::ostream & os, kdb::Key const & error)
{
	if (!error.getMeta<const kdb::Key>("error"))
	{
		// no error available
		return os;
	}

	try{
		error.getMeta<std::string>("error");
		os << "Error number " << error.getMeta<std::string>("error/number") << " occurred!" << std::endl;
		os << "Description: " << error.getMeta<std::string>("error/description") << std::endl;
		os << "Ingroup: " << error.getMeta<std::string>("error/ingroup") << std::endl;
		os << "Module: " << error.getMeta<std::string>("error/module") << std::endl;
		os << "At: " << error.getMeta<std::string>("error/file") << ":" << error.getMeta<std::string>("error/line") << std::endl;
		os << "Reason: " << error.getMeta<std::string>("error/reason") << std::endl;
	} catch (kdb::KeyMetaException const& e)
	{
		os << "Error meta data is not set correctly by a plugin" << std::endl;
	}
	return os;
}

inline std::ostream & printWarnings(std::ostream & os, kdb::Key const & error)
{
	if (!error.getMeta<const kdb::Key>("warnings"))
	{
		// no warnings were issued
		return os;
	}

	try{
		int nr = error.getMeta<int>("warnings");
		if (!nr)
		{
			os << "1 Warning was issued:" << std::endl;
		}
		else
		{
			os << nr+1 << " Warnings were issued:" << std::endl;
		}

		for (int i=0; i<=nr; i++)
		{
			std::ostringstream name;
			name << "warnings/#" << std::setfill('0') << std::setw(2) << i;
			// os << "\t" << name.str() << ": " << error.getMeta<std::string>(name.str()) << std::endl;
			os << "\tWarning number: " << error.getMeta<std::string>(name.str() + "/number") << std::endl;
			os << "\tDescription: " << error.getMeta<std::string>(name.str() + "/description") << std::endl;
			os << "\tIngroup: " << error.getMeta<std::string>(name.str() + "/ingroup") << std::endl;
			os << "\tModule: " << error.getMeta<std::string>(name.str() + "/module") << std::endl;
			os << "\tAt: " << error.getMeta<std::string>(name.str() + "/file") << ":"
				  << error.getMeta<std::string>(name.str() + "/line") << std::endl;
			os << "\tReason: " << error.getMeta<std::string>(name.str() + "/reason") << std::endl;
		}

	} catch (kdb::KeyMetaException const& e)
	{
		os << "Warnings meta data not set correctly by a plugin" << std::endl;
	}
}

#endif

#endif
