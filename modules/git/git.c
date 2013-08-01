/** \file git.c
 *  \brief A lunkwill git module
 */

#include <git2.h>
#include "../modules.h"

void *git_close_module(void *arg);
int git_get_request(void *request, struct html_ui* user_iface);

/** \brief Initializes the module */
char git_init_module(int id)
{
	printf("GIT MODULE INITIALIZED AT: /%d\n", id-1);
	session.module[id]=git_get_request;
	sighndlr_add(git_close_module, NULL);
	
	modules[id].id=id;
	modules[id].name="git";
	modules[id].description="A simple git module";
	
	return 1;
}


/** \brief Parses get requests */
int git_get_request(void *request, struct html_ui* user_iface)
{
	/** \todo Only works for 1 repository at the moment */
	const char *repo_path = "../.git";	
	//~ if (!config_lookup_string(&session.config, "REPOSITORY", &repo_path)){
		//~ fprintf(stderr, "Failed reading configuration\n");
		//~ return 1;
	//~ }
	
	git_repository *repo;
	if(git_repository_open(&repo, repo_path) != GIT_SUCCESS){
		fprintf(stderr, "Failed opening repository: '%s'\n", repo_path);
		return 1;
	}

	// Read HEAD on master
	char head_filepath[512];
	FILE *head_fileptr;
	char head_rev[41];
	
	strcpy(head_filepath, repo_path);
	
	if(strrchr(repo_path, '/') != (repo_path+strlen(repo_path)))
		strcat(head_filepath, "/refs/heads/master");
	else
		strcat(head_filepath, "refs/heads/master");
		
	
	if((head_fileptr = fopen(head_filepath, "r")) == NULL){
		fprintf(stderr, "Error opening '%s'\n", head_filepath);
		return 1;
	}
	
	if(fread(head_rev, 40, 1, head_fileptr) != 1){
		fprintf(stderr, "Error reading from '%s'\n", head_filepath);
		fclose(head_fileptr);
		return 1;
	}	
	
	fclose(head_fileptr);
	
	
	git_oid oid;
	git_revwalk *walker;
	git_commit *commit;

	if(git_oid_fromstr(&oid, head_rev) != GIT_SUCCESS){
		fprintf(stderr, "Invalid git object: '%s'\n", head_rev);
		return 1;
	}

	git_revwalk_new(&walker, repo);
	git_revwalk_sorting(walker, GIT_SORT_NONE);
	git_revwalk_push(walker, &oid);


	void *html_ptr;
	html_ptr=html_add_tag( \
		&user_iface->sidebar, \
		"<table>", NULL, "</table>");
	html_add_tag( \
		&html_ptr, \
		"<tr><td>", "master", "</td></tr>");
	html_add_tag( \
		&user_iface->main, \
		"<h1>", "git", "</h1><br>");
	html_add_tag( \
		&user_iface->main, \
		"", "<table><tr><td width=\"120\"><b>Commit</b></td><td width=\"650\"><b>Message</b></td><td width=\"600\"><b>Author</b></td><td><b>Time</b></td></tr>", "");


	const char *commit_message;
	char *commit_sha;
	const git_signature *commit_author;
	
	while(git_revwalk_next(&oid, walker) == GIT_SUCCESS){
		if(git_commit_lookup(&commit, repo, &oid)){
			fprintf(stderr, "Failed to lookup the next object\n");
			return 1;
		}

		commit_sha = git_oid_allocfmt(&oid);
		commit_message = git_commit_message(commit);
		commit_author = git_commit_committer(commit);

		char row[512];
		/** \todo Add ... if field content gets cut */
		sprintf(row, "<tr><td>%.7s</td><td>%.80s</td><td>%.40s &lt;%.60s&gt;</td><td>To be implemented</td></tr>",
				commit_sha, commit_message, commit_author->name, commit_author->email);

		html_add_tag( \
			&user_iface->main, \
			"", row, "");
			
		git_commit_free(commit);
	}


	html_add_tag( \
	&user_iface->main, \
	"", "</table>", "");

	git_revwalk_free(walker);
	git_repository_free(repo);

	return 0;
}


/** \brief Exits git module */
void *git_close_module(void *arg){
	// Closed successfully
	return NULL;
}

