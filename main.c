#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define MAX_CACHE_SIZE (1024*1024*8)
#define ELIMAGE_URL "http://paste.edisonnotes.com"

char *elimage_url = ELIMAGE_URL;

char* append_it(char *cptr, const char c)
{
	int len = strlen(cptr);
	cptr[len + 1] = cptr[len];
	cptr[len] = c;

	return cptr;
}

int main (int argc, char *argv[])
{
	unsigned char *cache;
	unsigned int ch; 
	cache = malloc( MAX_CACHE_SIZE );
	while ((ch = getchar()) != EOF)
	{
		append_it(cache, ch);
	}
    printf("%s \n", cache);

	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect:";

	curl_global_init(CURL_GLOBAL_ALL);

	curl_formadd(&formpost,
			&lastptr,
			CURLFORM_COPYNAME, "vimcn",
			CURLFORM_COPYCONTENTS, cache,
			CURLFORM_END);

	curl = curl_easy_init();
	/* initalize custom header list (stating that Expect: 100-continue is not
	 *        wanted */
	headerlist = curl_slist_append(headerlist, buf);
	if(curl) {
		/* what URL that receives this POST */
		curl_easy_setopt(curl, CURLOPT_URL, elimage_url);
		if ( (argc == 2) && (!strcmp(argv[1], "noexpectheader")) )
			/* only disable 100-continue header if explicitly requested */
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		res = curl_easy_perform(curl);

		/* always cleanup */
		curl_easy_cleanup(curl);

		/* then cleanup the formpost chain */
		curl_formfree(formpost);
		/* free slist */
		curl_slist_free_all (headerlist);
	}
	free(cache);
	return 0;

}
