using Microsoft.EntityFrameworkCore;
using Web_App.Tables;

namespace Web_App.DbContext;

public class Applicationdbcontext : Microsoft.EntityFrameworkCore.DbContext
{

        public Applicationdbcontext(DbContextOptions<Applicationdbcontext> options):base(options){}
        public DbSet<User> Users { get; set; }
}