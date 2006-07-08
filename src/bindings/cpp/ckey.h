#ifndef CKEY_H
#define CKEY_H

#include <string>
#include <stdint.h>

namespace ckdb {	
#include <kdb.h>
}

namespace kdb {

/**Key represents an elektra key.*/
class Key
{
public:
	Key ();
	// Key (const char name, ...);
	// varg
	~Key ();
	
	uint8_t getType();
	void setType(uint8_t type);

	void setFlag();
	void clearFlag();
	int getFlag();

	std::string getName();
	std::string getFullName();

	size_t getCommentSize();
	std::string getComment();
	void setComment(std::string comment);

	uid_t getUID();
	void setUID(uid_t uid);

	gid_t getGID();
	void setGID(gid_t gid);

	mode_t getAccess();
	void setAccess(mode_t mode);

	std::string getOwner();
	void setOwner(const std::string userDomain);

	size_t getDataSize();

	std::string getString();
	void setString(std::string newString);

	size_t getBinary(void *returnedBinary, size_t maxSize);
	size_t setBinary(const void *newBinary, size_t dataSize);

	bool getBool ();
	void setBool (bool b);
	
	char getChar ();
	void setChar (char c);
	
	unsigned char getUnsignedChar ();
	void setUnsignedChar (unsigned char uc);

	wchar_t getWChar_t ();
	void setWChar_t (wchar_t wc);

	int getInt ();
	void setInt (int i);

	unsigned int getUnsignedInt ();
	void setUnsignedInt (unsigned int ui);

	short getShort ();
	void setShort (short s);

	unsigned short getUnsignedShort ();
	void setUnsignedShort (unsigned short us);

	long getLong ();
	void setLong (long l);

	unsigned long getUnsignedLong ();
	void setUnsignedLong (unsigned long ul);

#if defined __GNUC__ && defined __USE_GNU
	long long getLongLong ();
	void setLongLong (long long ll);

	unsigned long long getUnsignedLongLong ();
	void setUnsignedLongLong (unsigned long long ull);
#endif
	
	float getFloat ();
	void setFloat (float f);
	
	double getDouble ();
	void setDouble (double d);

	long double getLongDouble ();
	void setLongDouble (long double ld);

	std::string getLink();
	void setLink(std::string target);

	time_t getMTime();
	time_t getATime();
	time_t getCTime();
	
	int isSystem();
	int isUser();
	
	int getNamespace();
	
	int isDir();
	int isLink();

	size_t toStream(FILE* stream, unsigned long options);
	// ostream!
protected:
	ckdb::Key * getKey () { return key; }
	void setKey (ckdb::Key * k) { key = k; }
private:
	ckdb::Key * key; // holds elektra key struct
};

} // end of namespace kdb

#endif
