#include <kdb.h>

#include <stdio.h>

void printError(char *what, Key const* parentKey)
{
	printf("%s \"%s\" returned error: %s and reason %s\n",
		what,
		keyName(parentKey),
		keyString(keyGetMeta(parentKey, "error/number")),
		keyString(keyGetMeta(parentKey, "error/reason"))
		);
}

int main()
{
	Key *parentKey = keyNew("", KEY_END);
	KDB *kdb = kdbOpen(parentKey);
	KeySet *ks = ksNew(0, KS_END);
	if (kdbGet(kdb, ks, parentKey) == -1)
	{
		printError("kdbGet", parentKey);
	}
	keyDel(parentKey);
	parentKey = keyNew("meta", KEY_META_NAME, KEY_END);
	if (kdbGet(kdb, ks, parentKey) == -1)
	{
		printError("kdbGet", parentKey);
	}
	keyDel(parentKey);
	parentKey = keyNew("/test/shell/somewhere", KEY_CASCADING_NAME, KEY_END);
	if (kdbGet(kdb, ks, parentKey) == -1)
	{
		printError("kdbGet", parentKey);
	}
	keyDel(parentKey);

	ksRewind(ks);
	Key *k;
	while ((k = ksNext(ks)))
	{
		printf ("%s = %s\n", keyName(k), keyString(k));
	}

	parentKey = keyNew("", KEY_END);
	if (kdbSet(kdb, ks, parentKey) == -1)
	{
		printError("kdbSet", parentKey);
	}
	keyDel(parentKey);
	parentKey = keyNew("meta", KEY_META_NAME, KEY_END);
	if (kdbSet(kdb, ks, parentKey) == -1)
	{
		printError("kdbSet", parentKey);
	}
	keyDel(parentKey);
	parentKey = keyNew("/test/shell/somewhere", KEY_CASCADING_NAME, KEY_END);
	if (kdbSet(kdb, ks, parentKey) == -1)
	{
		printError("kdbSet", parentKey);
	}

	ksDel(ks);
	kdbClose(kdb, 0);
	keyDel(parentKey);
}
