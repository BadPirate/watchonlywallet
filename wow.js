function reloadURL()
{
  var url = location.protocol + '//' + location.host + location.pathname + "?";
  for (coin in crypto_addresses)
  {
    addresses = crypto_addresses[coin];
    if (addresses.length > 0)
    {
      url = url + "btc=" + addresses.join(",");
    }
  }
  window.location.href = url;
}

function add_coin_address(coin, address)
{
  crypto_addresses[coin].push(address);
  reloadURL();
}

function delete_coin_address(coin, address)
{
  var index = crypto_addresses[coin].indexOf(address);
  if (index > -1)
  {
    crypto_addresses[coin].splice(index, 1);
    reloadURL();
  }
}
