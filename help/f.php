<?php
$data = file("way.txt");

var_dump($data);
echo "Moi!";
for ($i = 0; $i < count($data); ++$i) {
	$e = explode("|", $data[$i]);
	list($name, $latitude, $longitude) = $e;
	
	echo $e[0];
}