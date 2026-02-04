using System.ComponentModel.DataAnnotations;

namespace Web_App.Model;

public class AddModel
{
    [Required(ErrorMessage ="نام کاربری الزامی است")]
    public string UserName { get; set; }
    [Required(ErrorMessage ="کد کارت الزامی است")]
    public string CardId { get; set; }
}