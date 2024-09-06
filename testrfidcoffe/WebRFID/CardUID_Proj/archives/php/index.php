<!DOCTYPE html>
<html>
<head>
    <title>RFID Tag Database</title>
    <meta http-equiv="refresh" content="5"> <!-- Refresh every 5 seconds -->
    <style>
        table {
            width: 50%;
            border-collapse: collapse;
        }
        table, th, td {
            border: 1px solid black;
        }
        th, td {
            padding: 15px;
            text-align: left;
        }
    </style>
</head>
<body>

<h1>RFID Tag Database</h1>

<?php
$hostname = "localhost";
$username = "root";
$password = ":698yP9y?)*xVYK";
$database = "rfidtag";

$conn = new mysqli($hostname, $username, $password, $database);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT CardUID, Name FROM rfidrc522";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    echo "<table><tr><th>CardUID</th><th>Name</th></tr>";
    while($row = $result->fetch_assoc()) {
        echo "<tr><td>" . $row["CardUID"]. "</td><td>" . $row["Name"]. "</td></tr>";
    }
    echo "</table>";
} else {
    echo "0 results";
}
$conn->close();
?>

</body>
</html>
