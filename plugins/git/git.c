/** \file git.c
 *  \brief A lunkwill git module
 */

#include "git.h"

/** \brief Initializes the module */
int init_module(int id, struct module_info *m)
{
	char buf[64];
	sprintf(buf, "Git module intialized at %d", id);
	log_write(buf, LOG_INFO);

	m->id=id;
	m->name="git";
	m->func=answer_request;
	m->description="Just an example.";
	struct module_data* md=m->data=malloc(sizeof(struct module_data));

	sighndlr_add(close_module, md);
	return 0;
}

/** \brief Answer requests */
int answer_request(void *md, request *client_request)
{
	static pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&lock);

	const char *repo_path = b64_decode(client_request->module_request, B64_DEFAULT);

	git_repository *repo;
	if(git_repository_open(&repo, repo_path) != GIT_SUCCESS)
	{
		log_write("Failed opening repository: '%s'", LOG_ERR, repo_path);
		goto ERROR_SERVER;
	}

	// Read HEAD on master
	char head_filepath[512];
	FILE *head_fileptr;
	char head_rev[41];

	strncpy(head_filepath, repo_path, 512);

	if(strrchr(repo_path, '/') != (repo_path+strlen(repo_path)))
		strcat(head_filepath, "/refs/heads/master");
	else
		strcat(head_filepath, "refs/heads/master");


	if((head_fileptr = fopen(head_filepath, "r")) == NULL)
	{
		log_write("Error opening '%s'\n", LOG_ERR, head_filepath);
		goto ERROR_SERVER;
	}

	if(fread(head_rev, 40, 1, head_fileptr) != 1)
	{
		log_write("Error reading from '%s'\n", LOG_ERR, head_filepath);
		fclose(head_fileptr);
		goto ERROR_SERVER;
	}

	fclose(head_fileptr);


	git_oid oid;
	git_revwalk *walker;
	git_commit *commit;
	struct tm *commit_time_gmt;
	time_t commit_time;
	int commit_offset;

	if(git_oid_fromstr(&oid, head_rev) != GIT_SUCCESS)
	{
		log_write("Invalid git object: '%s'\n", LOG_ERR,head_rev);
		goto ERROR_SERVER;
	}

	git_revwalk_new(&walker, repo);
	git_revwalk_sorting(walker, GIT_SORT_TOPOLOGICAL);
	git_revwalk_push(walker, &oid);


	void *html_ptr;

	void *sha_html_ptr, *message_html_ptr, *author_html_ptr, *time_html_ptr;
	sha_html_ptr=html_add_tag(\
	                          &html_ptr, \
	                          "window.repo.sha=[", NULL, "];");

	message_html_ptr=html_add_tag(\
	                              &html_ptr, \
	                              "window.repo.message=[", NULL, "];");

	author_html_ptr=html_add_tag(\
	                             &html_ptr, \
	                             "window.repo.author=[", NULL, "];");

	time_html_ptr=html_add_tag(\
	                           &html_ptr, \
	                           "window.repo.time=[", NULL, "];");

	const char *commit_message;
	char *commit_sha;
	const git_signature *commit_author;
	char date[64];

	int i=0;

	while(git_revwalk_next(&oid, walker) == GIT_SUCCESS)
	{
		if(git_commit_lookup(&commit, repo, &oid))
		{
			log_write("Failed to lookup the next object\n", LOG_ERR);
			if(commit!=NULL)git_commit_free(commit);
			git_revwalk_free(walker);
			goto ERROR_SERVER;
		}

		commit_sha = git_oid_allocfmt(&oid);
		commit_message = git_commit_message(commit);
		commit_author = git_commit_committer(commit);
		commit_offset = git_commit_time_offset(commit);
		commit_time = git_commit_time(commit) + commit_offset*60;
		commit_time_gmt = gmtime(&commit_time);


		memset(date, 0, sizeof(date));
		strftime(date, 63, "%d. %B %G &nbsp; %R", commit_time_gmt);

		void *escaped, *author;

		html_add_tag(&sha_html_ptr,"'", commit_sha,"',");
		nfree(commit_sha);

		escaped=b64_encode((char *)commit_message,
		                   strlen((char *)commit_message),B64_DEFAULT);
		html_add_tag(&message_html_ptr,"'", escaped,"',");
		nfree(escaped);

		escaped=b64_encode((char *)commit_author->name,
		                   strlen(commit_author->name),B64_DEFAULT);
		author=html_add_tag(&author_html_ptr,"'", escaped, "',");
		nfree(escaped);

		escaped=b64_encode((char *)commit_author->email,
		                   strlen((char *)commit_author->email),B64_DEFAULT);
		html_add_tag(&author, " ", escaped, NULL);
		nfree(escaped);

		html_add_tag(&time_html_ptr,
		             "'", date, "',");

		git_commit_free(commit);
		i++;
	}

	git_revwalk_free(walker);
	git_repository_free(repo);

	nfree(repo_path);
	pthread_mutex_unlock(&lock);
	return 0;

ERROR_SERVER:
	git_repository_free(repo);
	nfree(repo_path);
	pthread_mutex_unlock(&lock);
	return 2;
}



/** \brief Exits git module */
void *close_module(void *arg)
{
	nfree(arg);
	return NULL;
}

