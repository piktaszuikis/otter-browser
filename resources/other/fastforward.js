(function(isSelectingTheBestLink, hrefTokens, classTokens, idTokens, textTokens)
{
	const REL_SCORE = 9000;
	const TEXT_SCORE = 100;
	const ID_SCORE = 11;
	const CLASS_SCORE = 10;
	const HREF_SCORE = 1;
	const THRESHOLD = 10;
	const DEBUG = false;

	function parseUrl(link)
	{
		var result = {
			pathname: link.pathname.split('/'),
			query: []
		};

		if (link.search)
		{
			var query = link.search.substr(1).split('&');
			query.every(function(item)
			{
				result.query.push(item.split('=')[0]);
			});
		}

		return result;
	}

	function calculateScore(value, tokens, defaultScore)
	{
		var score = 0;

		if (value)
		{
			for (var i = (tokens.length - 1); i >= 0; --i)
			{
				if (value.indexOf(tokens[i].value) !== -1)
				{
					score += tokens[i].score || defaultScore;
				}
			}
		}

		return score;
	}

	function calculateScoreForValues(values, tokens, defaultScore)
	{
		var score = 0;

		if (values && values.length > 0)
		{
			for(var i = (values.length - 1); i >= 0; --i)
			{
				score += calculateScore(values[i].toUpperCase(), tokens, defaultScore);
			}
		}

		return score;
	}

	var links = document.querySelectorAll('a:not([href^="javascript:"]):not([href="#"])');
	var scoredLinks = [];

	if (DEBUG)
	{
		console.log('FastForward DEBUG: Checking', links.length, 'links');
	}

	for (var i = (links.length - 1); i >= 0; --i)
	{
		var score = 0;
		var link = links[i];

		//check for rel
		if (links[i].rel && links[i].rel.indexOf('next') !== -1)
		{
			score += REL_SCORE;
		}

		//check for innerText, aria-label and alt
		score += calculateScore(Array.prototype.join.call([links[i].innerText, links[i].getAttribute('aria-label'), links[i].getAttribute('alt'), links[i].title], ' ').toUpperCase(), textTokens, TEXT_SCORE);

		//check for id
		score += calculateScore(links[i].id.toUpperCase(), idTokens, ID_SCORE);

		//check for class
		score += calculateScoreForValues(links[i].classList, classTokens, CLASS_SCORE);

		//check for location/href
		var url = parseUrl(links[i]);

		score += calculateScoreForValues(url.pathname, hrefTokens, HREF_SCORE)
		score += calculateScoreForValues(url.query, hrefTokens, HREF_SCORE)

		//Is link worthy?
		if (score > THRESHOLD)
		{
			if (isSelectingTheBestLink)
			{
				scoredLinks.push({score: score, link: links[i]});
			}
			else
			{
				if (DEBUG)
				{
					console.log('FastForward DEBUG: Found at least one link for FastForward. Score:', score);
				}

				return links[i].href;
			}
		}
	}

	if (scoredLinks.length > 0)
	{
		scoredLinks.sort(function(first, second)
		{
			return (second.score - first.score);
		});

		if (DEBUG)
		{
			for (var i = 0; i < scoredLinks.length; ++i)
			{
				console.log('FastForward DEBUG: ', scoredLinks[i].score, 'Url:', scoredLinks[i].link.outerHTML);
			}

			console.log('FastForward DEBUG: Choosing link with score', scoredLinks[0].score, scoredLinks[0].link.href);
		}

		return scoredLinks[0].link.href;
	}

	if (DEBUG)
	{
		console.log('FastForward DEBUG: No candidate links found!');
	}

	return null;
})({isSelectingTheBestLink}, {hrefTokens}, {classTokens}, {idTokens}, {textTokens})