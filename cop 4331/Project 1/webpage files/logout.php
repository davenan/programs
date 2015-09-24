<?php
    // This code logs the user out by destoying the session and redirecting to login page.
    session_start();
    session_unset();
    session_destroy();
    session_write_close();
    header('Location: index.php');
    die();
?>