<?php
$hostname = "localhost";
$username = "root";
$password = ":698yP9y?)*xVYK";
$database = "rfidtag";


$conn = mysqli_connect ($hostname,$username,$password,$database);

if (!$conn){
    die("Connection failed: ".mysqli_connect_error());

}
echo "Database connection is OK";

if(isset($_POST["CardUID"]) && isset($_POST["Name"])){

 $i = $_POST["CardUID"];
 $n  = $_POST["Name"];

 $sql = "INSERT INTO rfidrc522 (CardUID,Name) VALUES ('$i','$n')";
 if (mysqli_query($conn, $sql)){
    echo "<br>"."New record created successfully";
 } else {
    echo "ERROR:".$sql . "<br>".mysqli_error($conn);
 }
}
?>