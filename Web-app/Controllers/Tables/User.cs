using System.ComponentModel.DataAnnotations;

namespace Web_App.Tables;

public class User
{
    [Key]
    public int Id { get; set; }
    [MaxLength(80)]
    public string Username { get; set; }
    [MaxLength(80)]
    public string CardId { get; set; }
    [MaxLength(80)]
    public DateTime? OpenTime { get; set; }
    [MaxLength(80)]
    public DateTime? ExitTime { get; set; }
    public Byte? Status { get; set; }
}