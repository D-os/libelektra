/**
 * \file
 *
 * \brief A plugin that makes use of libaugeas to read and write configuration files
 *
 * \copyright BSD License (see doc/COPYING or http://www.libelektra.org)
 *
 */

#ifdef HAVE_KDBCONFIG_H
#include "kdbconfig.h"
#endif

/* used for asprintf */
#define _GNU_SOURCE

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <tests_plugin.h>
#include "rename.h"


static KeySet *createSimpleTestKeys()
{
	return ksNew (20,
			keyNew("user/tests/rename/will/be/stripped/key1", KEY_VALUE, "value1", KEY_END),
			keyNew("user/tests/rename/will/be/stripped/key2", KEY_VALUE, "value2", KEY_END),
			keyNew("user/tests/rename/will/be/stripped", KEY_VALUE, "value3", KEY_END),
			keyNew("user/tests/rename/will/not/be/stripped/key4", KEY_VALUE, "value4", KEY_END),
			KS_END);
}

static KeySet *createSimpleMetaTestKeys()
{
	return ksNew (20,
			keyNew("user/tests/rename/will/be/stripped/key1",
					KEY_VALUE, "value1",
					KEY_META, "rename/cut", "will/be/stripped",
					KEY_END),
			keyNew("user/tests/rename/will/be/stripped/key2",
					KEY_VALUE, "value2",
					KEY_META, "rename/cut", "will/be/stripped",
					KEY_END),
			keyNew("user/tests/rename/will/be/stripped", KEY_VALUE, "value3", KEY_END),
			keyNew("user/tests/rename/will/not/be/stripped/key4", KEY_VALUE, "value4", KEY_END),
			KS_END);
}

static void checkSimpleTestKeys(KeySet* ks)
{
	/* the first two keys should have been renamed */
	Key* key = ksLookupByName (ks, "user/tests/rename/key1", KDB_O_NONE);
	succeed_if(key, "key1 was not correctly renamed");
	key = ksLookupByName (ks, "user/tests/rename/key2", KDB_O_NONE);
	succeed_if(key, "key2 was not correctly renamed");
	/* the third key was not renamed because it would replace the parent key */
	key = ksLookupByName (ks, "user/tests/rename/will/be/stripped", KDB_O_NONE);
	succeed_if(key, "key3 was renamed but would replace the parent key");
	/* the fourth key was not renamed because the prefix did not match */
	key = ksLookupByName (ks, "user/tests/rename/will/not/be/stripped/key4", KDB_O_NONE);
	succeed_if(key, "key4 was renamed although its prefix did not match");
}

static void compareKeySets(KeySet* ks, KeySet* expected)
{
	succeed_if(ksGetSize (expected) == ksGetSize (ks), "KeySet on set does not contain the same amount of keys");
	Key* current;
	ksRewind (expected);
	while ((current = ksNext (expected)))
	{
		Key *key = ksLookup (ks, current, KDB_O_NONE);
		succeed_if (key, "Expected key was not found in KeySet");
		succeed_if (!strcmp(keyString(key), keyString(current)), "Value of key was modified");
	}
}

static void test_simpleCutOnGet () {
	Key *parentKey = keyNew ("user/tests/rename", KEY_END);
	KeySet *conf = ksNew (20,
			keyNew ("system/cut", KEY_VALUE, "will/be/stripped", KEY_END), KS_END);
	PLUGIN_OPEN("rename");

	KeySet *ks = createSimpleTestKeys();
	ksAppendKey(ks, parentKey);

	succeed_if(plugin->kdbGet (plugin, ks, parentKey) >= 1,
			"call to kdbGet was not successful");
	succeed_if(output_error (parentKey), "error in kdbGet");
	succeed_if(output_warnings (parentKey), "warnings in kdbGet");

	checkSimpleTestKeys (ks);

	keyDel (parentKey);
	ksDel(ks);
	PLUGIN_CLOSE ();
}


static void test_metaCutOnGet()
{
	Key *parentKey = keyNew ("user/tests/rename", KEY_END);
	KeySet *conf = ksNew (0, KS_END);
	PLUGIN_OPEN("rename");

	KeySet *ks = createSimpleMetaTestKeys();
	ksAppendKey(ks, parentKey);

	succeed_if(plugin->kdbGet (plugin, ks, parentKey) >= 1,
			"call to kdbGet was not successful");
	succeed_if(output_error (parentKey), "error in kdbGet");
	succeed_if(output_warnings (parentKey), "warnings in kdbGet");

	checkSimpleTestKeys(ks);

	keyDel (parentKey);
	ksDel(ks);
	PLUGIN_CLOSE ();

}

static void test_simpleCutRestoreOnSet () {
	Key *parentKey = keyNew ("user/tests/rename", KEY_END);
	Key *parentKeyCopy = keyDup(parentKey);
	KeySet *conf = ksNew (20,
			keyNew ("system/cut", KEY_VALUE, "will/be/stripped", KEY_END), KS_END);
	PLUGIN_OPEN("rename");

	KeySet *ks = createSimpleTestKeys();
	ksAppendKey(ks, parentKey);

	succeed_if(plugin->kdbGet (plugin, ks, parentKey) >= 1,
			"call to kdbGet was not successful");
	succeed_if(output_error (parentKey), "error in kdbGet");
	succeed_if(output_warnings (parentKey), "warnings in kdbGet");

	succeed_if(plugin->kdbSet (plugin, ks, parentKey) >= 1,
			"call to kdbSet was not successful");
	succeed_if(output_error (parentKey), "error in kdbSet");
	succeed_if(output_warnings (parentKey), "warnings in kdbSet");


	/* test that the keys have been correctly restored */
	KeySet *expected = createSimpleTestKeys();
	ksAppendKey(expected, parentKeyCopy);

	compareKeySets (ks, expected);
	keyDel (parentKey);
	keyDel (parentKeyCopy);
	ksDel(expected);
	ksDel(ks);
	PLUGIN_CLOSE ();
}

static void test_withoutConfig()
{
	Key *parentKey = keyNew ("user/tests/rename", KEY_END);
	Key *parentKeyCopy = keyDup(parentKey);
	KeySet *conf = ksNew (0, KS_END);
	PLUGIN_OPEN("rename");

	KeySet *ks = createSimpleTestKeys();
	ksAppendKey(ks, parentKey);

	succeed_if(plugin->kdbGet (plugin, ks, parentKey) >= 1,
			"call to kdbGet was not successful");
	succeed_if(output_error (parentKey), "error in kdbGet");
	succeed_if(output_warnings (parentKey), "warnings in kdbGet");

	KeySet *expected = createSimpleTestKeys();
	ksAppendKey(expected, parentKeyCopy);

	compareKeySets(ks, expected);
	keyDel (parentKey);
	keyDel (parentKeyCopy);
	ksDel(expected);
	ksDel(ks);
	PLUGIN_CLOSE ();
}

static void test_metaConfigTakesPrecedence()
{
	Key *parentKey = keyNew ("user/tests/rename", KEY_END);
	KeySet *conf = ksNew (20,
			keyNew ("system/cut", KEY_VALUE, "will/be", KEY_END), KS_END);
	PLUGIN_OPEN("rename");

	KeySet *ks = createSimpleMetaTestKeys();
	ksAppendKey(ks, parentKey);

	succeed_if(plugin->kdbGet (plugin, ks, parentKey) >= 1,
			"call to kdbGet was not successful");
	succeed_if(output_error (parentKey), "error in kdbGet");
	succeed_if(output_warnings (parentKey), "warnings in kdbGet");

	/* the first two keys should have been renamed by their metadata */
	Key* key = ksLookupByName (ks, "user/tests/rename/key1", KDB_O_NONE);
	succeed_if(key, "key1 was not correctly renamed");
	key = ksLookupByName (ks, "user/tests/rename/key2", KDB_O_NONE);
	succeed_if(key, "key2 was not correctly renamed");

	/* the third key should have been renamed by the global config */
	key = ksLookupByName (ks, "user/tests/rename/stripped", KDB_O_NONE);
	succeed_if(key, "key3 was renamed but would replace the parent key");

	/* the fourth key was not renamed because the prefix did not match */
	key = ksLookupByName (ks, "user/tests/rename/will/not/be/stripped/key4", KDB_O_NONE);
	succeed_if(key, "key4 was renamed although its prefix did not match");

	keyDel (parentKey);
	ksDel(ks);
	PLUGIN_CLOSE ();
}

static void test_keyCutNamePart()
{
	Key *parentKey = keyNew ("user/tests/rename", KEY_END);
	Key *result = elektraKeyCutNamePart(parentKey, parentKey, "wont/cut/this");
	succeed_if (!result, "parentKey was modified although it should have been ignored");

	/* don't allow to produce the parentKey with cutting */
	Key *testKey = keyNew ("user/tests/rename/wont/cut/this", KEY_END);
	result = elektraKeyCutNamePart(testKey, parentKey, "wont/cut/this");
	succeed_if (!result, "key was cut although it is identical with the parentKey afterwards");
	keyDel(testKey);

	/* cutting works correctly without trailing slash */
	testKey = keyNew ("user/tests/rename/will/cut/this/key1", KEY_END);
	result = elektraKeyCutNamePart(testKey, parentKey, "will/cut/this");
	succeed_if (result, "key1 was not cut")
	succeed_if (!strcmp(keyName(result), "user/tests/rename/key1"), "cutting key1 did not yield the expected result");
	keyDel(testKey);
	keyDel(result);

	/* cutting works correctly with trailing slash */
	testKey = keyNew ("user/tests/rename/will/cut/this/key1", KEY_END);
	result = elektraKeyCutNamePart(testKey, parentKey, "will/cut/this/");
	succeed_if (result, "key1 was not cut")
	succeed_if (!strcmp(keyName(result), "user/tests/rename/key1"), "cutting key1 did not yield the expected result");
	keyDel(testKey);
	keyDel(result);

	/* disallow leading slashes */
	testKey = keyNew ("user/tests/rename/wont/cut/this/key1", KEY_END);
	result = elektraKeyCutNamePart(testKey, parentKey, "/wont/cut/this");
	succeed_if (!result, "key was cut although it the cutpath contained a leading slash");
	keyDel(testKey);
	keyDel(parentKey);
}

int main(int argc, char** argv)
{
	printf ("RENAME       TESTS\n");
	printf ("==================\n\n");

	init (argc, argv);

	test_withoutConfig();
	test_simpleCutOnGet();
	test_simpleCutRestoreOnSet();
	test_metaCutOnGet();
	test_metaConfigTakesPrecedence();

	test_keyCutNamePart();

	printf ("\ntest_rename RESULTS: %d test(s) done. %d error(s).\n", nbTest,
			nbError);

	return nbError;
}
