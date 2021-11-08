void main()
{
    printf("Hello World1!\n");
    
    fork();
    printf("Hello World2!\n");
    // if (fork() == 0)
    // {
    //     printf("Hello World2!\n");
    // }
    printf("Hello World3!\n");
}