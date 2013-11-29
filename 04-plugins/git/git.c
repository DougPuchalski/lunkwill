/** \file git.c
 *  \brief A lunkwill git module
 */

#include "git.h"

/** \brief Initializes the module */
int init_module(int id, struct module_info *m)
{
	char buf[64];
	sprintf(buf, "Git module intialized at %d", id);
	log_write(buf, LOG_INFO, 0);

	m->id=id;
	m->name="git";
	m->func=answer_request;
	m->description="Just an example.";
	struct module_data* md=m->data=malloc(sizeof(struct module_data));

	sighndlr_add(close_module, md);
	return 0;
}

/** \brief Answer requests */
extern int answer_request(void *md, request *client_request)
{
	struct html_ui *user_iface=client_request->answer;

	/** \todo Only works for 1 repository at the moment */
	const char *repo_path = b64_decode(client_request->module_request, B64_URL);
	//~ if (!config_lookup_string(&session.config, "REPOSITORY", &repo_path)){
		//~ log_write("",LOG_ERR,1, "Failed reading configuration\n");
		//~ return 1;
	//~ }
	
	git_repository *repo;
	if(git_repository_open(&repo, repo_path) != GIT_SUCCESS){
		log_write("",LOG_ERR,1, "Failed opening repository: '%s'\n", repo_path);
		goto ERROR_SERVER;
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
		log_write("",LOG_ERR,1, "Error opening '%s'\n", head_filepath);
		git_repository_free(repo);
		goto ERROR_SERVER;
	}
	
	if(fread(head_rev, 40, 1, head_fileptr) != 1){
		log_write("",LOG_ERR,1, "Error reading from '%s'\n", head_filepath);
		fclose(head_fileptr);
		git_repository_free(repo);
		goto ERROR_SERVER;
	}
	
	fclose(head_fileptr);
	
	
	git_oid oid;
	git_revwalk *walker;
	git_commit *commit;
	struct tm *commit_time_gmt;
	time_t commit_time;
	int commit_offset;

	if(git_oid_fromstr(&oid, head_rev) != GIT_SUCCESS){
		log_write("",LOG_ERR,1, "Invalid git object: '%s'\n", head_rev);
		git_repository_free(repo);
		goto ERROR_SERVER;
	}

	git_revwalk_new(&walker, repo);
	git_revwalk_sorting(walker, GIT_SORT_TOPOLOGICAL);
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
		"<table><tr style=\"background-color: #C0C0C0;\">",
		"<td width=\"120\" style=\"padding-left: 5px;\"><b>Commit</b></td>" \
		"<td width=\"650\" style=\"padding-left: 5px;\"><b>Message</b></td>" \
		"<td width=\"600\" style=\"padding-left: 5px;\"><b>Author</b></td>" \
		"<td width=\"250\" style=\"padding-left: 5px;\"><b>Time</b></td></tr>", "");

	const char *commit_message;
	char *commit_sha;
	const git_signature *commit_author;
	
	char *color[] = {"#FFFFFF", "#E0E0E0"};
	
	int i=0;
	while(git_revwalk_next(&oid, walker) == GIT_SUCCESS){
		if(git_commit_lookup(&commit, repo, &oid)){
			log_write("",LOG_ERR,1, "Failed to lookup the next object\n");
			if(commit!=NULL)git_commit_free(commit);
			git_revwalk_free(walker);
			git_repository_free(repo);
			goto ERROR_SERVER;
		}

		commit_sha = git_oid_allocfmt(&oid);
		commit_message = git_commit_message(commit);
		commit_author = git_commit_committer(commit);
		commit_offset = git_commit_time_offset(commit);
		commit_time = git_commit_time(commit) + commit_offset*60;
		commit_time_gmt = gmtime(&commit_time);
		
		
		char date[64];
		strftime(date, 63, "%d. %B %G &nbsp; %R", commit_time_gmt);

		void *row, *escaped, *author;

		html_add_tag( \
			&user_iface->main, \
			"<tr style=\"background-color: ", color[i%2], ";\" class=\"git_commits\">");

		row=html_add_tag( \
			&user_iface->main, \
			NULL, NULL, "</tr>");

		html_add_tag( &row,
				"<td style=\"padding-left: 5px;\">",
				commit_sha,
				"</td>" );
		nfree(commit_sha);			
			
		html_add_tag( &row,
				"<td style=\"padding-left: 5px;\">",
				escaped=html_escape((char *)commit_message),
				"</td>" );
		nfree(escaped);
			
		author=html_add_tag( &row,
			"<td style=\"padding-left: 5px;\">",
			escaped=html_escape((char *)commit_author->name),
			"</td>" );
		nfree(escaped);
			
		html_add_tag( &author,
			" &lt;",
			escaped=html_escape((char *)commit_author->email),
			"&gt; " );
		nfree(escaped);
			
		html_add_tag( &row,
			"<td style=\"padding-left: 5px;\">",
			date,
			"</td>" );
			
		git_commit_free(commit);
		i++;
	}


	html_add_tag( \
	&user_iface->main, \
	"", "</table>", "");

	git_revwalk_free(walker);
	git_repository_free(repo);
	
	nfree(repo_path);
	return 0;
	
ERROR_SERVER:;
	void *x=html_flush(&((struct html_ui*)client_request->answer)->base,1);
	nfree(x);
	nfree(repo_path);
	html_add_tag(&((struct html_ui*)client_request->answer)->base, HTTP_451, "", "");
	return 2;
}


/** \brief Exits git module */
void *close_module(void *arg){
	nfree(arg);
	return NULL;
}

