using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Web_App.DbContext;
using Web_App.Model;
using Web_App.Tables;

namespace Web_App.Controllers;

public class HomeController : Controller
{
    private static string _CardId = "";
    private readonly Applicationdbcontext _dbcontext;

    public HomeController(Applicationdbcontext dbcontext)
    {
       _dbcontext = dbcontext;
    }
    [Route("/")]
    public IActionResult Index()
    {
        var Show_Users = _dbcontext.Users.ToList();
       return View(Show_Users);
    }
    [HttpGet("/Add")]
    public IActionResult Add()
    {
        if (_CardId != null)
        {
            ViewData["CardId"] = _CardId;
        }
        return View();
    }
    [HttpGet("/CardId/{CardId}")]
    public IActionResult CardId(String CardId)
    {
        var Check_CardId = _dbcontext.Users.SingleOrDefault(u => u.CardId == CardId);
        if (CardId != null && Check_CardId == null)
        {
            _CardId = CardId;
            return Ok("Success");
        }
        else
        {
            return Ok("User Found");
        }
    }
    [HttpGet("/Edit/{CardId}")]
    public IActionResult Edit(String CardId)
    {
        var UserUid = _dbcontext.Users.SingleOrDefault(u => u.CardId == CardId);
        if (UserUid.Status == null)
        {
            UserUid.OpenTime = DateTime.Now;
            UserUid.ExitTime = null;
            UserUid.Status = 1;
            _dbcontext.Users.Update(UserUid);
            _dbcontext.SaveChanges();
            return Ok("Hello " + UserUid.Username);
        }
        else if (UserUid.Status == 1)
        {
            if ((DateTime.Now - UserUid.OpenTime.Value).TotalMinutes < 2)
            {
                return Ok("Access Denied");
            }
            else if((DateTime.Now - UserUid.OpenTime.Value).TotalMinutes >= 2)
            {
                UserUid.ExitTime = DateTime.Now;
                UserUid.Status=2;
                _dbcontext.Users.Update(UserUid);
                _dbcontext.SaveChanges();
                return Ok("GoodBye " + UserUid.Username);
            }
        }
        else if (UserUid.Status == 2)
        {
            if ((DateTime.Now - UserUid.ExitTime.Value).TotalMinutes < 2)
            {
                return Ok("Access Denied");
            }
            else if((DateTime.Now - UserUid.ExitTime.Value).TotalMinutes >= 2)
            {
                UserUid.OpenTime = DateTime.Now;
                UserUid.Status=1;
                _dbcontext.Users.Update(UserUid);
                _dbcontext.SaveChanges();
                return Ok("Hello " + UserUid.Username);
            }
        }
        return Ok("Invalid State");
    }
    [HttpPost("/Add")]
    public IActionResult Add(AddModel AddCardId)
    {
        var Username = _dbcontext.Users.SingleOrDefault(u => u.Username == AddCardId.UserName);
        var CardId = _dbcontext.Users.SingleOrDefault(u => u.CardId == AddCardId.CardId);
        
        if (ModelState.IsValid)
        {
            if (Username != null && CardId != null)
            {
                ModelState.AddModelError("Username" ,"نام کاربری تکراری است" );
                ModelState.AddModelError("CardId" ,"کد کارت تکراری است" );
                return View();
            }
            else if (Username != null)
            {
                ModelState.AddModelError("Username","نام کاربری  تکراری است");
                return View();
            }
            else if(CardId !=null)
            {
                ModelState.AddModelError("CardId" ,"کد کارت  تکراری است" );
                return View();
            }
            else
            {
                var NewUser = new User
                {
                    Username = AddCardId.UserName,
                    CardId = AddCardId.CardId,
                    OpenTime = null,
                    ExitTime = null,
                    Status = null,
                };
                _dbcontext.Users.Add(NewUser);
                _dbcontext.SaveChanges();
                return RedirectToAction("Index", "Home");
            }
            
        }
        else
        {
            return View();
        }
    }
    
}