#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int closeVoting(FILE*);

int main(int argc, char** argv) {
  char* home = getenv("HOME");
  size_t home_len = strlen(home);
  char* sender = getenv("SENDER");
  size_t sender_len = strlen(sender);
  char* owner_path = malloc(home_len + sender_len + 2);  // "/owner\0"
  strncpy(owner_path, home, home_len);
  strncpy(owner_path + home_len, "/owner\0", 7);

  FILE* fd = fopen(owner_path, "rb");
  if (fd == NULL) {
    // constructor.

    // set owner, state.
    fd = fopen(owner_path, "wb");
    fprintf(fd, "1");
    fwrite(sender, strlen(sender), 1, fd);
    fclose(fd);

    // set candidates.
    int num_candidates;
    scanf("%d", &num_candidates);
    char* candidate_path = malloc(home_len + 15);  // "/<number>\0"
    strncpy(candidate_path, home, home_len);
    candidate_path[home_len] = '/';
    for (; num_candidates >= 0; num_candidates--) {
      snprintf(candidate_path + home_len + 1, 14, "%d\0", num_candidates);
      fd = fopen(candidate_path, "wb");
      fprintf(fd, "%d\0", 0);
      fclose(fd);
    }

    return 0;
  }

  // check state.
  char state = fgetc(fd);
  if (state != '1') {
    // if closed, return winning candidate.
    int winner;
    fscanf(fd, "%d", &winner);
    printf("%d", winner);
    return 0;
  }

  int vote;
  scanf("%d", &vote);
  // check if owner is closing poll.
  if (vote == -1) {
    return closeVoting(fd);
  }

  // has this sender already voted?
  strncpy(owner_path + home_len + 1, sender, sender_len);
  fd = fopen(owner_path, "rb");
  if (fd != NULL) {
    // already voted.
    return 1;
  }

  // cast a vote.
  fd = fopen(owner_path, "wb");
  if (fd == NULL) {
    // write failed.
    return 2;
  }
  fprintf(fd, "%d", vote);
  fclose(fd);

  // increment the vote count for the candidate.
  snprintf(owner_path + home_len + 1, 14, "%d\0", vote);
  fd = fopen(owner_path, "rb");
  if (fd == NULL) {
    // no such candidate.
    return 1;
  }
  fscanf(fd, "%d", &vote);
  fclose(fd);

  vote++;

  fd = fopen(owner_path, "wb");
  fprintf(fd, "%d\0", vote);
  fclose(fd);

  return 0;
}

int closeVoting(FILE* fd) {
  char* home = getenv("HOME");
  size_t home_len = strlen(home);
  char* sender = getenv("SENDER");
  char* candidate_path = malloc(home_len + 15);  // "/<number>\0"

  int vote;
  int idx;
  int max_idx = 0;
  int max_vote = 0;

  // check ownership.
  for (vote = 0; vote < strlen(sender); vote++) {
    if (getc(fd) != sender[vote]) {
      return 1;
    }
  }
  fclose(fd);

  // find max vote
  vote = 0;
  strncpy(candidate_path, home, home_len);
  candidate_path[home_len] = '/';
  while (1) {
    snprintf(candidate_path + home_len + 1, 14, "%d\0", idx);
    fd = fopen(candidate_path, "rb");
    if (fd == NULL) {
      break;
    }
    fscanf(fd, "%d", &vote);
    fclose(fd);
    if (vote > max_vote) {
      max_vote = vote;
      max_idx = idx;
    }
    idx++;
  }

  // update owner with winner.
  strncpy(candidate_path + home_len, "/owner\0", 7);
  fd = fopen(candidate_path, "wb");
  fprintf(fd, "0");
  fprintf(fd, "%d\0", max_idx);
  fclose(fd);

  return 0;
}